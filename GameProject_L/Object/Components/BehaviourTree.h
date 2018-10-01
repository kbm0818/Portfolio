#pragma once
class BehaviourTree {
public:
	class Node {  // This class represents each node in the behaviour tree.
	public:
		virtual bool Update() = 0;
		virtual ~Node() {};
	};

	class CompositeNode : public Node {  //  This type of Node follows the Composite Pattern, containing a list of other Nodes.
	private:
		vector<Node*> children;
	public:
		virtual ~CompositeNode() {
			vector<Node*>::iterator iter;
			for (iter = children.begin(); iter != children.end(); iter++)
			{
				SAFE_DELETE(*iter);
			}
		}
		const vector<Node*>& GetChildren() const { return children; }
		void AddChild(Node* child) { children.emplace_back(child); }
		void AddChildren(initializer_list<Node*>&& newChildren) { for (Node* child : newChildren) AddChild(child); }
		template <typename CONTAINER>
		void AddChildren(const CONTAINER& newChildren) { for (Node* child : newChildren) AddChild(child); }
	protected:
		vector<Node*> childrenShuffled() const { vector<Node*> temp = children;  random_shuffle(temp.begin(), temp.end());  return temp; }
	};

	class Selector : public CompositeNode {
	public:
		virtual ~Selector() {}
		virtual bool Update() override {
			for (Node* child : GetChildren()) {  // The generic Selector implementation
				if (child->Update())  // If one child succeeds, the entire operation Update() succeeds.  Failure only results if all children fail.
					return true;
			}
			return false;  // All children failed so the entire Update() operation fails.
		}
	};

	class RandomSelector : public CompositeNode {  // RandomSelector operates as a Selector, but in a random order instead of from first child to last child (Adds more unpredictability to the behaviour when a there is no clear preferred order of execution).
	public:
		virtual ~RandomSelector() {}
		virtual bool Update() override {
			for (Node* child : childrenShuffled()) {  // The order is shuffled
				if (child->Update())
					return true;
			}
			return false;
		}
	};

	class Sequence : public CompositeNode {
	public:
		virtual ~Sequence() {}
		virtual bool Update() override {
			for (Node* child : GetChildren()) {  // The generic Sequence implementation.
				if (!child->Update())  // If one child fails, then enter operation Update() fails.  Success only results if all children succeed.
					return false;
			}
			return true;  // All children suceeded, so the entire Update() operation succeeds.
		}
	};

	class DecoratorNode : public Node {  // Function is either to transform the result it receives from its child node's status, to terminate the child, or repeat processing of the child, depending on the type of decorator node.
	private:
		Node* child;  // Only one child allowed
	protected:
		Node* GetChild() const { return child; }
	public:
		virtual ~DecoratorNode() { SAFE_DELETE(child); }
		void setChild(Node* newChild) { child = newChild; }
	};

	class Root : public DecoratorNode {
	public:
		virtual ~Root() {}
	private:
		friend class BehaviourTree;
		virtual bool Update() override { return GetChild()->Update(); }
	};

	class Inverter : public DecoratorNode {  // Inverts the result of the child. A child fails and it will return success to its parent, or a child succeeds and it will return failure to the parent.
	public:
		virtual ~Inverter() {}
	private:
		virtual bool Update() override { return !GetChild()->Update(); }
	};

	class Succeeder : public DecoratorNode {  // A succeeder will always return success, irrespective of what the child node actually returned. These are useful in cases where you want to process a branch of a tree where a failure is expected or anticipated, but you don¡¯t want to abandon processing of a sequence that branch sits on.
	public:
		virtual ~Succeeder() {}
	private:
		virtual bool Update() override { GetChild()->Update();  return true; }
	};

	class Failer : public DecoratorNode {  // The opposite of a Succeeder, always returning fail.  Note that this can be achieved also by using an Inverter and setting its child to a Succeeder.
	public:
		virtual ~Failer() {}
	private:
		virtual bool Update() override { GetChild()->Update();  return false; }
	};

	class Repeater : public DecoratorNode {  // A repeater will reprocess its child node each time its child returns a result. These are often used at the very base of the tree, to make the tree to Update continuously. Repeaters may optionally Update their children a set number of times before returning to their parent.
	public:
		virtual ~Repeater() {}
	private:
		int numRepeats;
		static const int NOT_FOUND = -1;
		Repeater(int num = NOT_FOUND) : numRepeats(num) {}  // By default, never terminate.
		virtual bool Update() override {
			if (numRepeats == NOT_FOUND)
				while (true) GetChild()->Update();
			else {
				for (int i = 0; i < numRepeats - 1; i++)
					GetChild()->Update();
				return GetChild()->Update();
			}
		}
	};

	class RepeatUntilFail : public DecoratorNode {  // Like a repeater, these decorators will continue to reprocess their child. That is until the child finally returns a failure, at which point the repeater will return success to its parent.
	public:
		virtual ~RepeatUntilFail() {}
	private:
		virtual bool Update() override {
			while (GetChild()->Update()) {}
			return true;
		}
	};

	template <typename T>
	class StackNode : public Node {
	public:
		virtual ~StackNode() {
			while (s.empty())
			{
				SAFE_DELETE(s.pop());
			}
		}
	protected:
		stack<T*>& s;  // Must be reference to a stack to work.
		StackNode(stack<T*>& s) : s(s) {}
	};

	template <typename T>
	class PushToStack : public StackNode<T> {  // Specific type of leaf (hence has no child).
	private:
		T*& item;
	public:
		virtual ~PushToStack() {};
		PushToStack(T*& t, stack<T*>& s) : StackNode<T>(s), item(t) {}
	private:
		virtual bool Update() override {
			this->s.push(item);
			return true;
		}
	};

	template <typename T>
	class GetStack : public StackNode<T> {  // Specific type of leaf (hence has no child).
	private:
		const stack<T*>& obtainedStack;
		T* object;
	public:
		virtual ~GetStack() {};
		GetStack(stack<T*>& s, const stack<T*>& o, T* t = nullptr) : StackNode<T>(s), obtainedStack(o), object(t) {}
	private:
		virtual bool Update() override {
			this->s = obtainedStack;
			if (object)
				this->s.push(object);
			return true;
		}
	};

	template <typename T>
	class PopFromStack : public StackNode<T> {  // Specific type of leaf (hence has no child).
	private:
		T*& item;
	public:
		virtual ~PopFromStack() {};
		PopFromStack(T*& t, stack<T*>& s) : StackNode<T>(s), item(t) {}
	private:
		virtual bool Update() override {
			if (this->stack.empty())
				return false;
			item = this->s.top();
			cout << "Trying to Get through door #" << item->doorNumber << "." << endl;  // template specialization with T = Door needed for this line actually
			this->s.pop();
			return true;
		}
	};

	template <typename T>
	class StackIsEmpty : public StackNode<T> {  // Specific type of leaf (hence has no child).
	public:
		virtual ~StackIsEmpty() {};
		StackIsEmpty(stack<T*>& s) : StackNode<T>(s) {}
	private:
		virtual bool Update() override {
			return this->stack.empty();
		}
	};

	template <typename T>
	class SetVariable : public BehaviourTree::Node {  // Specific type of leaf (hence has no child).
	private:
		T *&variable, *&object;  // Must use reference to pointer to work correctly.
	public:
		virtual ~SetVariable() {
			SAFE_DELETE(variable);
			SAFE_DELETE(object);
		};
		SetVariable(T*& t, T*& obj) : variable(t), object(obj) {}
		virtual bool Update() override {
			variable = object;
			cout << "The door that was used to Get in is door #" << variable->doorNumber << "." << endl;  // template specialization with T = Door needed for this line actually
			return true;
		};
	};

	template <typename T>
	class IsNull : public BehaviourTree::Node {  // Specific type of leaf (hence has no child).
	private:
		T*& object;  // Must use reference to pointer to work correctly.
	public:
		virtual ~IsNull() {
			SAFE_DELETE(object);
		};
		IsNull(T*& t) : object(t) {}
		virtual bool Update() override { return !object; }
	};
private:
	Root* root;
public:
	BehaviourTree() : root(new Root) {}
	~BehaviourTree() {
		SAFE_DELETE(root);
	}
	void setRootChild(Node* rootChild) const { root->setChild(rootChild); }
	bool Update() const { return root->Update(); }
};