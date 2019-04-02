#include "ClauseGraph.h"

ClauseGraph::ClauseGraph(std::vector<std::pair<std::string, std::pair<std::string, std::string>>> st,
	std::vector<std::pair<std::string, std::string>> w,
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> p, 
	std::vector<std::string> select) {
	suchThatClauses = st;
	withClauses = w;
	patternClauses = p;
	selectClause = select;

}

void ClauseGraph::groupClause() {
	groupByClauseType(st);
	groupByClauseType(w);
	groupByClauseType(p);
	int check = suchThatClauses.size() + withClauses.size() + patternClauses.size();
	std::pair<clauseType, int> cl;
	bool trivial;
	while (check != 0) {
		cl = getFirstCl(); //function for the more efficient? get from PKB statistics
		trivial = mapClauses(std::unordered_set<std::pair<std::string, std::pair <std::string, std::string >>>(), cl, false);
		if (trivial) {
			//add to non trivial sets
		}
		else {
			//add to trivial set
		}
	}
}

void ClauseGraph::groupByClauseType(clauseType t) {
	std::vector<std::string> synLst;
	if (t == st) {
		for (int i = 0; i < suchThatClauses.size(); i++) {
			synLst = extractSuchThatSyn(i);
			mapSynonyms(synLst, std::make_pair(st, i));
		}
	} else if (t == p) {
		for (int i = 0; i < patternClauses.size(); i++) {
			synLst = extractPatternSyn(i);
			mapSynonyms(synLst, std::make_pair(p, i));
		}
	} else {
		for (int i = 0; i < withClauses.size(); i++) {
			synLst = extractWithSyn(i);
			mapSynonyms(synLst, std::make_pair(w, i));
		}
	}
}

void ClauseGraph::mapSynonyms(std::vector<std::string> synLst, std::pair<clauseType, int> cl) {
	clMap[cl] = synLst;
	for (std::vector<std::string>::iterator it = synLst.begin(); it != synLst.end(); ++it) {
		if (synMap.find(*it) == synMap.end()) {
			//synMap[*it] = std::vector<std::pair<clauseType, int>>();
			synMap[*it].push_back(cl);
		} else {
			//group index for cl0 = group index for cl1
			synMap.at[*it].insert(cl);
		}
	}
}

bool ClauseGraph::mapClauses(std::vector <std::pair<clauseType, int>> set, std::pair<clauseType, int> cl, bool trivial) {
	std::vector<std::string> synLst = clMap[cl];
	for (std::vector<std::string>::iterator it = synLst.begin(); it != synLst.end(); ++it) {
		//choose the more optimal clause
		//
		//check if *it is part of select
		if (true) {
			trivial = true;
		}
		set.push_back(cl);
		cl = getOptimal(synMap[*it])
		synMap[*it].remove(cl);
		clMap[cl].remove(*it);
		mapClauses(set, cl);
	}
}

std::vector<std::string> ClauseGraph::extractSuchThatSyn(int index) {
	std::vector<std::string> synLst;
	std::string f = suchThatClauses.at(index).second.first;
	std::string s = suchThatClauses.at(index).second.second;
	if (isSynonym(f)) synLst.push_back(f);
	if (isSynonym(s)) synLst.push_back(s);
	return synLst;
}

std::vector<std::string> ClauseGraph::extractPatternSyn(int index) {
	std::vector<std::string> synLst;
	std::string f = patternClauses.at(index).first;
	std::string s = patternClauses.at(index).second.first;
	if (isSynonym(f)) synLst.push_back(f);
	if (isSynonym(s)) synLst.push_back(s);
	return synLst;
}

std::vector<std::string> ClauseGraph::extractWithSyn(int index) {
	std::vector<std::string> synLst;
	std::string f = withClauses.at(index).first;
	std::string s = withClauses.at(index).second;
	if (isWithSynonym(f)) synLst.push_back(f);
	if (isWithSynonym(s)) synLst.push_back(s);
	return synLst;
}

bool isSynonym(std::string str) {
	bool res = false;
	if (str.find('"') == std::string::npos) {
		res = true;
	}
	return res;
}

bool isWithSynonym(std::string str) {
	bool res = false;
	if (str.find('.') == std::string::npos) {
		res = true;
	}
	return res;
}

