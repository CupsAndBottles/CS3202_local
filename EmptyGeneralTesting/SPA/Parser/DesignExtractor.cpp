#include "DesignExtractor.h"
#include "..\Program\Program.h"
#include "..\PKB\Follows.h"
#include "..\PKB\ProcTable.h"
#include "..\PKB\StmtTypeTable.h"
#include "..\PKB\Uses.h"
#include "..\PKB\Modifies.h"
#include "..\PKB\Calls.h"
#include "..\PKB\Next.h"
#include "..\PKB\Parent.h"
#include "..\PKB\NextBip.h"
#include "..\PKB\NodeTypeTable.h"
#include "..\QueryProcessor\Grammar.h"

#include <vector>
#include <map>
#include <set>

/*
proctable
	first and last stmts of each procedure
Modifies/Uses
	Move code for recursive calls from PKB to here.
calls
	get calls stmts from stmttypetable
	get procedures containing said stmts from proctable
	build call tree.
procedure - modifies/uses
	Build call tree
	reduce SCCs to vertices
	compile uses and modifies for resulting graph
Next
	use follows to calculate
	return tail of next chain
		start on stmt 1
			when encountering a while loop
				perform recursive call to parse subtree
				connect tail to head, proceed.
			when encountering if/then/else block
				perform recursive call to parse subtrees
				connect tails to follows(ifstmt, next)
*/

void ComputeNodeTypeTable();
void ComputeModifiesAndUses();
void ComputeCalls();
void ComputeModifiesAndUsesForProcedures();
void ComputeNext();
void ComputeNextBip();

void DesignExtractor::Extract() {
	ComputeNodeTypeTable();
	ComputeCalls();
	ComputeNext();
	ComputeNextBip();
	ComputeModifiesAndUsesForProcedures();
	ComputeModifiesAndUses();
}

void ComputeNodeTypeTable() {
	// flatten nodes and iterate through them
	// ast has no loops if following only child links.

	NodeTypeTable::Initialise();
	
	vector<TNode*> currentSet;
	currentSet.push_back(&Program::GetASTRootNode());

	while (currentSet.size() > 0) {
		vector<TNode*> nextSet;
		for each (TNode* nodeptr in currentSet) {
			NodeTypeTable::Insert(nodeptr);
			for each (TNode* child in nodeptr->GetChildren()) {
				nextSet.push_back(child);
			}
		}
		currentSet = nextSet;
	}
}

void ComputeModifiesAndUsesForProcedures() {
	// Use calls in PKB and shrink SCCs
	// we shall use an implementation of Tarjan's strongly connected components algorithm

	struct ProcInfo {
		// check if node is visited by accessing the depth
		// check if node is in component by checking the componentIndex
		ProcInfo(int procIndex = -1) 
			: procIndex(procIndex), componentIndex(0)
			, depth(0), minReacheableDepth(-1) {}
		int procIndex;
		int componentIndex;
		int depth;
		int minReacheableDepth;
	};

	class TarjanHelper {

	public:
		int componentCounter;
		TarjanHelper() {
			depthCounter, componentCounter = 1;
			// TODO find an easier way of getting this out
			for each (string procedure in ProcTable::GetAllProcNames()) {
				int procIndex = ProcTable::GetIndexOfProc(procedure);
				procMap[procIndex] = ProcInfo(procIndex);
				int i = procMap[procIndex].minReacheableDepth;
				procedures.push_back(procIndex);
			}
		}

		map<int, ProcInfo> ComputeSCC() {
			for each (int procedure in procedures) {
				if(procMap[procedure].depth == 0) {
					Tarjan(procedure);
				}
			}
			return procMap;
		}

	private:
		map<int, ProcInfo> procMap;
		vector<int> SCCStack;
		vector<int> procedures;
		int depthCounter;

		void Tarjan(int proc) {
			ProcInfo& currentProcedure = procMap[proc];
			currentProcedure.depth = depthCounter;
			currentProcedure.minReacheableDepth = depthCounter;
			depthCounter++;
			SCCStack.push_back(proc);
			// map children of currentProcedure
			for each (int procedure in Calls::GetProcsCalledBy(proc)){
				ProcInfo& calledProcedure = procMap[procedure];
				if (calledProcedure.depth == 0) { // procedure has not been mapped
					Tarjan(procedure); // add proc to stack
					currentProcedure.minReacheableDepth = 
						(calledProcedure.minReacheableDepth < currentProcedure.minReacheableDepth) 
						? calledProcedure.minReacheableDepth : currentProcedure.minReacheableDepth;
				} else {
					// do not add proc to stack
					currentProcedure.minReacheableDepth = 
						(calledProcedure.minReacheableDepth < currentProcedure.minReacheableDepth) 
						? calledProcedure.minReacheableDepth : currentProcedure.minReacheableDepth;
				}
			}

			// SCC generation time
			if (currentProcedure.depth == currentProcedure.minReacheableDepth) {
				ProcInfo& nextProc = procMap[SCCStack.back()];
				while (nextProc.procIndex != currentProcedure.procIndex) {
					nextProc.componentIndex = componentCounter;
					SCCStack.pop_back();
					nextProc = procMap[SCCStack.back()];
				}
				procMap[SCCStack.back()].componentIndex = componentCounter;
				SCCStack.pop_back();
				componentCounter++; // one component done, on to the next
			}
		}
	};

	// TODO can someone teach me to declare unnamed objects?
	TarjanHelper helper;
	map<int, ProcInfo> callGraph = helper.ComputeSCC();
	
	// iterate through procs, generating SCCs
	// for each proc, look at all procs calling said proc
	//	if the calling proc has a different component number, said proc is parent
	set<int> componentsWithoutChildren; for(int i = 1; i < helper.componentCounter; i++) componentsWithoutChildren.insert(i);
	map<int, vector<int>> componentMap;
	map<int, set<int>> componentGraph;
	typedef map<int, ProcInfo>::iterator c_iter;
	for (c_iter i = callGraph.begin(); i != callGraph.end(); i++) {
		int procIndex = i->first;
		int componentIndex = i->second.componentIndex;
		componentMap[componentIndex].push_back(procIndex);
		// find parents
		for each (int procedure in Calls::GetProcsCalling(procIndex)) {
			int callingComponentIndex = callGraph[procedure].componentIndex;
			if (callingComponentIndex != componentIndex) {
				componentGraph[componentIndex].insert(callingComponentIndex);
				componentsWithoutChildren.erase(callingComponentIndex);
			}
		}
	}

	// walk through SCC graph
	// for each vertex/component, all procs use and modify the set union of all variables in that component
	// for each child of vertex, vertex uses/modifies variables of children
	// recursive implementation
	typedef map<int, vector<int>>::iterator c_map_iter;
	typedef map<int, set<int>>::iterator c_grh_iter;
	// iterate through each component
	// compute us/mo for procs within components first
	for (c_map_iter i = componentMap.begin(); i != componentMap.end(); i++) {
		if (i->second.size() == 1) continue;
		set<int> usedVars;
		set<int> modifiedVars;
		// sum up the vars for all procs in the component
		for each (int procedure in i->second) {
			vector<int> uses = Uses::GetVarUsedByProc(procedure);
			vector<int> modifies = Modifies::GetProcModifyingVar(procedure);
			usedVars.insert(uses.begin(), uses.end());
			modifiedVars.insert(modifies.begin(), modifies.end());
		}
		// make all procs inherit the vars summed up
		for each (int procedure in i->second) {
			for each (int var in usedVars) {
				Uses::SetProcUsesVar(procedure, var);
			}
			for each (int var in modifiedVars) {
				Modifies::SetProcModifiesVar(procedure, var);
			}
		}
	}

	// loop through leaves
	// all direct parents inherit properties of children
	// clean leaves and add parents of current set to that
	// loop from top

	// loop through all leaves
	while (componentsWithoutChildren.size() != 0) {
		set<int> currentSet;
		for each(int leaf in componentsWithoutChildren) {
			// get the first proc in the child component
			int firstProcedure = componentMap[leaf].front();
			vector<int> usedVars = Uses::GetVarUsedByProc(firstProcedure);
			vector<int> modifiedVars = Modifies::GetProcModifyingVar(firstProcedure);

			set<int>& currentParents = componentGraph[leaf];
			// all direct parents inherit properties of children
			for each(int parent in currentParents) {
				for each (int procedure in componentMap[parent]) {
					for each (int var in usedVars) {
						Uses::SetProcUsesVar(procedure, var);
					}
					for each (int var in modifiedVars) {
						Modifies::SetProcModifiesVar(procedure, var);
					}
				}
			}
			// add direct parents to set
			currentSet.insert(currentParents.begin(), currentParents.end());
		}
		// loop from top
		componentsWithoutChildren = currentSet;
	}
}

set<int> ConnectStmtList(int startPoint) {
	// seeks follows connections until none can be found
	// if while is found
	//	recursively call this to process the stmt list
	//	connect last stmt to first one
	// if else is found
	//	recursively call this twice for each stmt list
	//	connect last stmts to next one
	// return last stmt found (or a set)

	set<int> prevStmts;
	int currentStmt = startPoint;
	while (currentStmt != -1) {
		// set connections
		for each (int prevStmt in prevStmts) Next::SetNext(prevStmt, currentStmt);
		prevStmts.clear();
		// process current stmt
		if (StmtTypeTable::CheckIfStmtOfType(currentStmt, IF)) {

			TNode& ifStmt = Program::GetStmtFromNumber(currentStmt);
			int startOfThenBlock = ifStmt.GetChild(1).GetChild(0).GetLineNumber();
			int startOfElseBlock = ifStmt.GetChild(2).GetChild(0).GetLineNumber();
			Next::SetNext(currentStmt, startOfThenBlock);
			Next::SetNext(currentStmt, startOfElseBlock);

			set<int> thenBlockEnds = ConnectStmtList(startOfThenBlock);
			set<int> elseBlockEnds = ConnectStmtList(startOfElseBlock);
			prevStmts.insert(thenBlockEnds.begin(), thenBlockEnds.end());
			prevStmts.insert(elseBlockEnds.begin(), elseBlockEnds.end());

		} else if (StmtTypeTable::CheckIfStmtOfType(currentStmt, WHILE)) {

			TNode& whileStmt = Program::GetStmtFromNumber(currentStmt);
			int startOfLoop = whileStmt.GetChild(1).GetChild(0).GetLineNumber();
			Next::SetNext(currentStmt, startOfLoop);
			
			set<int> loopEnds = ConnectStmtList(startOfLoop);
			for each (int stmt in loopEnds) Next::SetNext(stmt, currentStmt);
			prevStmts.insert(currentStmt);

		} else {
			prevStmts.insert(currentStmt);
		}
		// goto next stmt
		currentStmt = Follows::GetFollowsAfter(currentStmt);
	}
	return prevStmts;
}

void ComputeNext() {
	// gets all procedures
	// loops through all procedures, connecting the stmts
	for each (string procName in ProcTable::GetAllProcNames()) {
		int procIndex = ProcTable::GetIndexOfProc(procName);
		set<int> finalStmts = ConnectStmtList(ProcTable::GetFirstStmtNoOfProc(procIndex));
		for each (int stmt in finalStmts) {
			NextBip::setEndOfProc(procIndex, stmt);
		}
	}
}

void ComputeCalls() {
	// inserting calls
	for each (int call in StmtTypeTable::GetAllStmtsOfType(CALL)) {
		TNode& callNode = Program::GetStmtFromNumber(call);
		int callingProc = ProcTable::GetProcOfStmt(call);
		int calledProc = ProcTable::GetIndexOfProc(callNode.GetContent());
		Calls::SetCalls(callingProc, calledProc);
	}
}

void ComputeModifiesAndUses() {
	/* Remember that
		while x {		\\ 1
			while y {	\\ 2
				z = 1;	\\ 3
			}
		}

		1 only modifies 3, and not 2 as well.
	*/

	// get all assignment statements
	// map to direct parents
	// direct parents inherit properties of children
	// set direct parents as new children
	// loop

	vector<int> assignmentStmts = StmtTypeTable::GetAllStmtsOfType(ASSIGN);
	vector<int> callStmts = StmtTypeTable::GetAllStmtsOfType(CALL);

	set<int> currentChildren;
	currentChildren.insert(assignmentStmts.begin(), assignmentStmts.end());
	currentChildren.insert(callStmts.begin(), callStmts.end());
	
	while (currentChildren.size() != 0) {
		set<int> parents;

		for each (int stmt in currentChildren) {
			int parent = Parent::GetParentOf(stmt);
			if (parent == -1) continue;
			vector<int> usedVars, modifiedVars;

			if (StmtTypeTable::GetStmtTypeOf(stmt) == CALL) {
				// should only encounter this code on the first pass. Maybe can optimise
				int proc = ProcTable::GetIndexOfProc(Program::GetStmtFromNumber(stmt).GetContent());
				usedVars = Uses::GetVarUsedByProc(proc);
				// compute modifies and uses for calls here as well
				modifiedVars = Modifies::GetVarModifiedByProc(proc);
				for each (int var in usedVars) {
					Uses::SetStmtUsesVar(stmt, var);
				}
				for each (int var in modifiedVars) {
					Modifies::SetStmtModifiesVar(stmt, var);
				}
			} else { // assign or while or whatever
				usedVars = Uses::GetVarUsedByStmt(stmt);
				modifiedVars = Modifies::GetVarModifiedByStmt(stmt);
			}

			for each (int var in usedVars) {
				Uses::SetStmtUsesVar(parent, var);
			}
			for each (int var in modifiedVars) {
				Modifies::SetStmtModifiesVar(parent, var);
			}
			parents.insert(parent);
		}

		currentChildren = parents;
	}
}

void ComputeNextBip() {
	for each (int call in StmtTypeTable::GetAllStmtsOfType(CALL)) {
		TNode& callNode = Program::GetStmtFromNumber(call);
		int calledProc = ProcTable::GetIndexOfProc(callNode.GetContent());
		NextBip::setReturnPoint(calledProc, call);
	}

}