#include "ParentStorage.h"

unordered_map<int, pRelationships> ParentStorage::parentTable;
unordered_set< pair<int, int>, intPairhash> ParentStorage::parent_ChildPairList;
unordered_set< pair<int, int>, intPairhash> ParentStorage::anc_DescPairList;
unordered_set<int> ParentStorage::parentList;
unordered_set<int> ParentStorage::childrenList;
unordered_set<int> ParentStorage::rootList;

ParentStorage::ParentStorage()
{
}

/*
	Adds the parent relation into the various lists in the storage
	Returns false if	1) the pair is already stored
						2) the child already has another parent
*/
bool ParentStorage::addParent_Child(int parent, int child)
{
	// if Parent-Child Pair is already added
	if (!parent_ChildPairList.emplace(pair<int, int>(parent, child)).second)
	{
		return false;
	}

	// if child exist in parentTable and has parent already initialized
	auto itrpr = parentTable.emplace(child, pRelationships{ parent, {}, {}, {} });
	if (!itrpr.second && parentTable.find(child)->second.parent != 0)
	{
		parent_ChildPairList.erase(pair<int, int>(parent, child));
		return false;
	}
	else if (!itrpr.second)		// if child exist but does not have parent initialized
	{
		itrpr.first->second.parent = parent;
		if (rootList.find(child) != rootList.end())	//if child was a root
		{
			rootList.erase(child);
			int root = parent;
			while (parentTable.find(root)->second.parent != 0)	//while root has a parent
			{
				root = parentTable.find(root)->second.parent;
			}
			rootList.emplace(root);
		}
	}

	// if a new parent statement is added into the table, add to the root list
	if (parentTable.emplace(parent, pRelationships{ 0, {child}, {}, {} }).second)
	{
		rootList.emplace(parent);
	}
	else
	{
		parentTable.find(parent)->second.children.emplace(child);
	}

	parentList.emplace(parent);
	childrenList.emplace(child);
	return true;
}

/*
	Sets "ancestors" of the descendant
	Each ancestor - descendant pair is entered into anc_DescPairList
	If descendant already has a list of ancestors, it is not replaced and it return false
*/
bool ParentStorage::setAncestors(int descendant, unordered_set<int> ancestors)
{
	if (parentTable.find(descendant)->second.ancestors.size() != 0)
	{
		return false;
	}
	parentTable.find(descendant)->second.ancestors = ancestors;

	for (auto itr = ancestors.cbegin(); itr != ancestors.cend(); ++itr)
	{
		anc_DescPairList.emplace(pair<int, int>(*itr, descendant));
	}
	return true;
}

/*
	Sets "descendants" of the ancestor
	Each ancestor - descendant pair is entered into anc_DescPairList
	If ancestor already has a list of descendants, it is not replaced and it return false
*/
bool ParentStorage::setDescendants(int ancestor, unordered_set<int> descendants)
{
	if (parentTable.find(ancestor)->second.descendants.size() != 0)
	{
		return false;
	}
	parentTable.find(ancestor)->second.descendants = descendants;

	for (auto itr = descendants.cbegin(); itr != descendants.cend(); ++itr)
	{
		anc_DescPairList.emplace(pair<int, int>(ancestor, *itr));
	}
	return true;
}

bool ParentStorage::isEmpty()
{
	return parentTable.size() == 0;
}

bool ParentStorage::isParent(int stm)
{
	return parentList.find(stm) != parentList.end();
}

bool ParentStorage::isChild(int stm)
{
	return childrenList.find(stm) != childrenList.end();
}

bool ParentStorage::containsAnc_Desc(pair<int, int> pair)
{
	return anc_DescPairList.find(pair) != anc_DescPairList.end();
}

/*
	return the statement that is the parent of the statement specified
	return 0 if 'stm' is not found
*/
int ParentStorage::getParentOf(int stm)
{
	if (parentTable.find(stm) != parentTable.end())
	{
		return parentTable.at(stm).parent;
	}
	return 0;
}

/*
	return the list of statements that is the children of the statement specified
	return an empty set if 'stm' is not found
*/
unordered_set<int> ParentStorage::getChildrenOf(int stm)
{
	if (parentTable.find(stm) != parentTable.end())
	{
		return parentTable.at(stm).children;
	}
	return {};
}

/*
	return the list of statements that is the ancestor of the statement specified
	return an empty set if 'stm' is not found
*/
unordered_set<int> ParentStorage::getAncestorsOf(int stm)
{
	if (parentTable.find(stm) != parentTable.end())
	{
		return parentTable.at(stm).ancestors;
	}
	return {};
}

/*
	return the list of statements that is the descendants of the statement specified
	return an empty set if 'stm' is not found
*/
unordered_set<int> ParentStorage::getDescendantsOf(int stm)
{
	if (parentTable.find(stm) != parentTable.end())
	{
		return parentTable.at(stm).descendants;
	}
	return {};
}

unordered_set<int> ParentStorage::getParentList()
{
	return parentList;
}

unordered_set<int> ParentStorage::getChildrenList()
{
	return childrenList;
}

unordered_set<pair<int, int>, intPairhash> ParentStorage::getParent_ChildList()
{
	return parent_ChildPairList;
}

unordered_set<pair<int, int>, intPairhash> ParentStorage::getAnc_DescList()
{
	return anc_DescPairList;
}

unordered_set<int> ParentStorage::getRootList()
{
	return rootList;
}
