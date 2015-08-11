#ifndef OPERATIONSTACK
#define OPERATIONSTACK

#include <deque>
#include <QtOpenGL>
#include <vector>
#include <exception>
#include <stdexcept>
#include "treeinfo.h"

using namespace std;


struct TreeNode
{
	QString name;//»­²¼±àºÅ
	TreeInfo info;
	QVector3D position;
	QVector3D angle;
	QVector3D scale;
	TreeNode(QString na, TreeInfo in, QVector3D p, QVector3D a, QVector3D s)
		:name(na), info(in), position(p), angle(a), scale(s)
	{
	}
};

struct ActionNode
{
	int operation;
	vector<TreeNode> changedTrees;
	int count()
	{
		return changedTrees.size();
	}
	static const int ADD = 1;
	static const int REMOVE = 2;
	static const int ALTER = 3;
};

class OperationStack
{
private:
	deque<ActionNode> undo;
	deque<ActionNode> redo;
	const int SIZE;
	bool lock = false;
public:
	void PushToUndo(ActionNode x)
	{
		while (undo.size() >= SIZE)
		{
			undo.pop_front();
			lock = true;
		}
		undo.push_back(x);
	}

	ActionNode PopFromUndo()
	{
		if (undo.empty())
		{
			throw range_error("Stack is Empty£¡");
		}
		ActionNode x = undo.back();
		undo.pop_back();
		return x;
	}

	void PushToRedo(ActionNode x)
	{
		while (redo.size() >= SIZE)
		{
			redo.pop_front();
		}
		redo.push_back(x);
	}

	ActionNode PopFromRedo()
	{
		if (redo.empty())
		{
			throw range_error("Stack is Empty£¡");
		}
		ActionNode x = redo.back();
		redo.pop_back();
		return x;
	}

	void ClearRedo()
	{
		redo.clear();
	}

	void ClearUndo()
	{
		undo.clear();
	}

	bool IsUndoEmpty()
	{
		return undo.empty();
	}

	bool IsRedoEmpty()
	{
		return redo.empty();
	}

	bool IsFileChange()
	{
		return !(IsUndoEmpty() && !lock);
	}
public:
	OperationStack(int size=30) :SIZE(size){}
};

#endif