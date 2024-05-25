#include <iostream>
using namespace std;

//узел дерева
struct Treenode {
	int key = 0;
	Treenode* left = nullptr;
	Treenode* right = nullptr;
	Treenode* up = nullptr;
	unsigned short depth = 1;
	unsigned short minDepth = 0;
};

//структура дерева
struct Tree {

	//структура очереди
	struct Queue {
		// узел односвязного списка
		struct Node {
			Treenode* data = nullptr;
			Node* next = nullptr;
		};

		int cnt = 0;
		Node* first = nullptr;
		Node* last = nullptr;

		//функция добавления элемента в очередь
		void queue(Treenode* Treenode) {
			Node* newNode = new Node();
			newNode->data = Treenode;
			newNode->next = nullptr;
			if (first == nullptr) {
				first = newNode;
				last = newNode;
			}
			else {
				last->next = newNode;
				last = newNode;
			}
			cnt++;
		}

		//функция извлечения элемента из очереди 
		int unqueue() {
			if (first == nullptr) {
				return 0;
			}
			int data = first->data->key;
			Node* temp = first;
			first = first->next;
			if (first == nullptr) {
				last = nullptr;
			}
			delete temp;
			cnt--;
			return data;
		}

		//функция подсчета количества значений в очереди
		int count() {
			return cnt;
		}
	};

	Treenode* root = nullptr;
	int cnt = 0;

	enum Order { Prefix, Infix, Postfix, LevelsUpLeft, LevelsUpRight, LevelsDownLeft, LevelsDownRight};

	//функция добавления элемента
	void add(int data) {
		if (cnt == 0) {
			Treenode* newNode = new Treenode;
			newNode->key = data;
			cnt++;
			root = newNode;
			return;
		}
		Treenode* parent = findParent(data);
		if (parent == nullptr) {
			return;
		}
		Treenode* newNode = new Treenode;
		cnt++;
		newNode->up = parent;
		newNode->key = data;
		if (data > parent->key) {
			parent->right = newNode;
		}
		else {
			parent->left = newNode;
		}
		depthFixer(newNode);
	}

	//удаление элемента
	void remove(int key) {
		Treenode* temp = findNode(key);
		if (temp == nullptr) {
			return;
		}
		Treenode* parent = temp->up;
		if ((temp->right != nullptr) && (temp->left != nullptr)) {
			parent = temp;
			Treenode* minimum = temp->right;
			while (minimum->left != nullptr) {
				parent = minimum;
				minimum = minimum->left;
			}
			temp->key = minimum->key;
			temp = minimum;
		}
		Treenode* child = nullptr;
		if (temp->left != nullptr) {
			child = temp->left;
		}
		else {
			child = temp->right;
		}
		if (temp != root) {
			if (parent->left == temp) {
				parent->left = child;
			}
			else if (parent->right == temp) {
				parent->right = child;
			}
		}
		else {
			root = child;
		}
		if (child != nullptr) {
			child->up = parent;
		}
		depthFixer(temp);
		delete temp;
		cnt--;
	}

	//функция удаления
	void clear(Treenode* temp) {
		if (temp == nullptr) {
			return;
		}
		clear(temp->left);
		clear(temp->right);
		delete temp;
		cnt--;
	}

	//удаление всех элементов
	void clear() {
		clear(root);
		root = nullptr;
	}

	//содержит ли дерево заданный элемент
	bool contains(int data) {
		Treenode* temp = root;
		while (temp != nullptr) {
			if (temp->key == data) {
				return true;
			}
			else if (temp->key > data) {
				temp = temp->left;
			}
			else {
				temp = temp->right;
			}
		}
		return false;
	}

	//количество элементов коллекции
	int count() {
		return cnt;
	}

	//обход дерева по уровням
	void traverse(int* array, int& index, Order order, Treenode* temp) {
		if (temp == nullptr) {
			return;
		}
		Queue* queue = new Queue;
		queue->queue(temp);
		while (index != cnt) {
			for (int i = queue->count(); i > 0; i--) {
				if (order == LevelsUpLeft || order == LevelsDownRight) {
					if (queue->first->data->left != nullptr) {
						queue->queue(queue->first->data->left);
					}
				}
				if (queue->first->data->right != nullptr) {
					queue->queue(queue->first->data->right);
				}
				if (order == LevelsUpRight || order == LevelsDownLeft) {
					if (queue->first->data->left != nullptr) {
						queue->queue(queue->first->data->left);
					}
				}
				array[index] = queue->unqueue();
				index++;
			}
		}
		if (order == LevelsDownLeft || order == LevelsDownRight) {
			reverse(array, array + cnt);
		}
	}

	//конвертация в массив в зависимости от ордера перичисления
	void req(int* array, int& index, Order order, Treenode* temp) {
		if (temp == nullptr) {
			return;
		}
		if (order == Prefix) {
			array[index] = temp->key;
			index++;
		}
		req(array, index, order, temp->left);
		if (order == Infix) {
			array[index] = temp->key;
			index++;
		}
		req(array, index, order, temp->right);
		if (order == Postfix) {
			array[index] = temp->key;
			index++;
		}
	}

	//конвертация дерева в массив с инфиксным ордером перечисления
	int* ToArray(Order order = Infix) {
		int* arr = new int[count()];
		int index = 0;
		if (order > 2) {
			traverse(arr, index, order, root);
		}
		if (order < 3) {
			req(arr, index, order, root);
		}
		return arr;
	}

	//поиск узла по значению
	Treenode* findNode(int key) {
		Treenode* temp = root;
		while (temp != nullptr) {
			if (key == temp->key) {
				return temp;
			}
			if (key < temp->key) {
				temp = temp->left;
			}
			else {
				temp = temp->right;
			}
		}
		return nullptr;
	}

	// поиск родителя заданного узла
	Treenode* findParent(int key) {
		Treenode* parent = nullptr;
		Treenode* temp = root;
		while (temp != nullptr) {
			if (key == temp->key) {
				return nullptr;
			}
			parent = temp;
			if (key < temp->key) {
				temp = temp->left;
			}
			else {
				temp = temp->right;
			}
		}
		return parent;
	}

	//обновление значений глубины 
	void depthFixer(Treenode* temp) {
		while (temp != nullptr) {
			unsigned short left = 0;
			unsigned short right = 0;
			unsigned short minLeft = 0;
			unsigned short minRight = 0;
			if (temp->left != nullptr) {
				left = temp->left->depth;
				minLeft = temp->left->minDepth;
			}
			if (temp->right != nullptr) {
				right = temp->right->depth;
				minRight = temp->right->minDepth;
			}
			temp->depth = max(left, right) + 1;
			temp->minDepth = (min(minLeft, minRight) + 1);
			temp = temp->up;
		}
	}

	//двойной поворот
	bool doubleTurn(Treenode* temp) {
		if (getDepth(temp, true) - 1 > getDepth(temp, false)) {
			if (getDepth(temp->left, true) < getDepth(temp->left, false)) {
				turn(temp->left, true);
			}
			turn(temp, false);
			return true;
		}
		else if (getDepth(temp, false) - 1 > getDepth(temp, true)) {
			if (getDepth(temp->right, false) < getDepth(temp->right, true)) {
				turn(temp->right, false);
			}
			turn(temp, true);
			return true;
		}
		return false;
	}

	//максимальная глубина правой
	int getDepth(Treenode* temp, bool isLeft) {
		if (isLeft) {
			if (temp->left != nullptr) {
				return temp->left->depth;
			}
		}
		else {
			if (temp->right != nullptr) {
				return temp->right->depth;
			}
		}
		return 0;
	}

	//минимальная глубина ветки
	int getMinDepth(Treenode* temp, bool isLeft) {
		if (isLeft) {
			if (temp->left != nullptr) {
				return temp->left->minDepth;
			}
		}
		else {
			if (temp->right != nullptr) {
				return temp->right->minDepth;
			}
		}
		return 0;
	}

	//снижение глубины дерева
	void additionalTurn(Treenode* temp) {
		if (temp == nullptr) {
			return;
		}
		if (getDepth(temp, true) > getDepth(temp, false)) {
			if (getMinDepth(temp, true) == getDepth(temp, false)) {
				turn(temp->left, true);
				if (turn(temp, false)) {
					temp = temp->up;
				}
				doubleTurn(temp->left);
				doubleTurn(temp->right);
			}
		}
		else if (getDepth(temp, true) < getDepth(temp, false)) {
			if (getDepth(temp, true) == getMinDepth(temp, false)) {
				turn(temp->right, false);
				if (turn(temp, true)) {
					temp = temp->up;
				}
				doubleTurn(temp->right);
				doubleTurn(temp->left);
			}
		}
	}

	//поворот относительно узла
	bool turn(Treenode* temp, bool isLeft) {
		if (temp == nullptr) {
			return false;
		}
		Treenode* parent = temp->up;
		Treenode* child = nullptr;
		Treenode* grandchild = nullptr;
		if (isLeft) {
			child = temp->right;
			if (child == nullptr) {
				return false;
			}
			grandchild = child->left;
			child->left = temp;
			temp->right = grandchild;
		}
		else {
			child = temp->left;
			if (child == nullptr) {
				return false;
			}
			grandchild = child->right;
			child->right = temp;
			temp->left = grandchild;
		}
		if (parent == nullptr) {
			root = child;
		}
		else {
			if (parent->left == temp) {
				parent->left = child;
			}
			else {
				parent->right = child;
			}
		}
		if (grandchild != nullptr) {
			grandchild->up = temp;
		}
		temp->up = child;
		child->up = parent;
		depthFixer(temp);
		return true;
	}

	//поворот влево
	void ToLeft(int key) {
		Treenode* temp = findNode(key);
		turn(temp, true);
	}

	//поворот вправо
	void ToRight(int key) {
		Treenode* temp = findNode(key);
		turn(temp, false);
	}

	//убирает "выступы"
	void renegadeBalancer(Treenode* temp) {
		if (temp == nullptr) {
			return;
		}
		while (abs(getDepth(temp, true) - getDepth(temp, false)) > 1) {
			doubleTurn(temp);
		}
	}

	//балансировка
	void Balance(Treenode* temp = nullptr) {
		if (temp == nullptr) {
			temp = root;
		}
		if (temp == nullptr) {
			return;
		}
		if (temp->depth < 3) {
			return;
		}
		if (temp->left != nullptr) {
			Balance(temp->left);
		}
		if (temp->right != nullptr) {
			Balance(temp->right);
		}
		if (temp->depth < 3) {
			return;
		}
		if (doubleTurn(temp)) {
			renegadeBalancer(temp);
		}
		else {
			additionalTurn(temp);
		}
	}
};


int main()
{
	Tree tree;

	setlocale(LC_ALL, "ru");

	cout << "Введите число для:" << endl;
	cout << "1. добавления элементов в бинарное дерево" << endl << "2. удаления элемента из бинарного дерева" << endl;
	cout << "3. удаления всех элементов коллекции" << endl << "4. определения, содержится ли данный элемент в дереве" << endl;
	cout << "5. подсчета элементов коллекции" << endl;
	cout << "6. конвертации бинарного дерева в динамический массив" << endl;
	cout << "7. поворота дерева влево относительно узла с заданным значением" << endl;
	cout << "8. поворота дерева вправо относительно узла с заданным значением" << endl;
	cout << "9. балансировки дерева" << endl;
	cout << "0. завершения работы программы" << endl;

	int key, check;
	int* arr;

	while (true) {
		int key, numb, cnt = 0;
		cin >> numb;
		switch (numb) {
		case 1:
			cout << "Введите числа для заполнения дерева. Для окончания ввода введите число 0." << endl;
			while (true) {
				cin >> key;
				if (key == 0) {
					break;
				}
				tree.add(key);
			}
			break;
		case 2:
			cout << "Введите числа для удаления его из дерева: " << endl;
			cin >> key;
			tree.remove(key);
			break;
			break;
		case 3:
			tree.clear();
			cout << "В дереве нет элементов" << endl;
			break;
		case 4:
			cout << "Введите числа для нахождения их в дереве. Для окончания ввода введите число 0." << endl;
			while (true) {
				cin >> check;
				if (check == 0) {
					break;
				}
				if (tree.contains(check)) {
					cout << "Это число есть в дереве!" << endl;
				}
				else {
					cout << "Этого числа нет в дереве:(" << endl;
				}
			}
			break;
		case 5:
			cout << "Количество элементов в дереве: " << tree.count() << endl;
			break;
		case 6:
			arr = tree.ToArray();
			cout << "элементы дерева: ";
			for (int i = 0; i < tree.count(); i++) {
				cout << arr[i] << " ";
			}
			cout << endl;
			break;
		case 7:
			cout << "Введите число, относительно которого необходимо сделать поворот: ";
			cin >> key;
			tree.ToLeft(key);
			break;
		case 8:
			cout << "Введите число, относительно которого необходимо сделать поворот: ";
			cin >> key;
			tree.ToRight(key);
			break;
		case 9:
			tree.Balance();
			break;
		case 0:
			return 0;
		default:
			cout << "Ошибка! Введите корректный номер команды!!!" << endl;
			break;
		}
	}
}