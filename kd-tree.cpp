#include <bits/stdc++.h>
#include <cstdint>
using namespace std;
using namespace chrono;

const int k = 11;

struct Node
{
    float point[k];
    int dimension;
    Node *left, *right;
    Node(float arr[])
    {
        for (int i = 0; i < k; ++i)
            point[i] = arr[i];

        left = right = NULL;
    }
};

struct PointWithDistance
{
    float point[k];
    double distance;

    PointWithDistance(int arr[], double dist) : distance(dist)
    {
        for (int i = 0; i < k; ++i)
            point[i] = arr[i];
    }
};

Node *insertRec(Node *root, float point[], unsigned depth)
{
    if (root == NULL)
        return new Node(point);

    unsigned cd = depth % k;
    root->dimension = cd;

    if (point[cd] < (root->point[cd]))
        root->left = insertRec(root->left, point, depth + 1);
    else
        root->right = insertRec(root->right, point, depth + 1);

    return root;
}

Node *insert(Node *root, float point[])
{
    return insertRec(root, point, 0);
}

Node *insert_per(Node *root, float point[], unsigned depth)
{
    return insertRec(root, point, depth);
}

bool arePointsSame(float point1[], float point2[])
{
    for (int i = 0; i < k; ++i)
    {
        if (point1[i] != point2[i])
        {
            return false;
        }
    }
    return true;
}

Node *findMin(Node *root, unsigned depth, unsigned cd)
{
    if (root == nullptr)
    {
        return nullptr;
    }

    cd = depth % k;

    if (root->left == nullptr)
    {
        return root;
    }

    return findMin(root->left, depth + 1, cd);
}

Node *find_min(Node *root, unsigned cd)
{
    return findMin(root, 0, cd);
}

Node *findParent(Node *root, float point[])
{
    Node *parent = nullptr;
    unsigned cd = 0;

    while (root != nullptr && !arePointsSame(root->point, point))
    {
        cd = cd % k;

        parent = root;

        if (point[cd] < root->point[cd])
            root = root->left;
        else
            root = root->right;

        cd++;
    }
    return parent;
}

Node *search_nodo(Node *root, float point[], unsigned depth)
{
    // Base cases
    if (root == nullptr || arePointsSame(root->point, point))
        return root;

    // Current dimension is computed using current depth and total
    // dimensions (k)
    unsigned cd = depth % k;

    // Compare point with root with respect to cd (Current dimension)
    // cout << "dim " << cd << ": " << point[cd] << " " << root->point[cd] << endl;
    if (point[cd] < root->point[cd])
        return search_nodo(root->left, point, depth + 1);
    else
        return search_nodo(root->right, point, depth + 1);
}

// busqueda
Node *search_node(Node *root, float point[])
{
    return search_nodo(root, point, 0);
}

void getSubtreeWithoutNodeHelper(Node *current, Node *nodeToAvoid, std::vector<Node *> &result)
{
    if (current == nullptr)
    {
        return;
    }

    getSubtreeWithoutNodeHelper(current->left, nodeToAvoid, result);

    if (!arePointsSame(current->point, nodeToAvoid->point))
    {
        result.push_back(current);
    }

    getSubtreeWithoutNodeHelper(current->right, nodeToAvoid, result);
}

std::vector<Node *> getSubtreeWithoutNode(Node *subtreeRoot, Node *nodeToAvoid)
{
    std::vector<Node *> result;
    getSubtreeWithoutNodeHelper(subtreeRoot, nodeToAvoid, result);
    return result;
}

Node *borrar(Node *root, float point[])
{
    Node *parent = findParent(root, point);
    Node *current = search_node(root, point);

    if (current == nullptr)
    {
        return root;
    }
    int dim = current->dimension;
    Node *true_son = nullptr;
    vector<Node *> hijos_subarbol;
    vector<Node *> hijos_subarbol_dos;

    if (current->right == nullptr)
    {
        true_son = find_min(current->left, 0);
        hijos_subarbol = getSubtreeWithoutNode(current->left, true_son);
        if (true_son)
        {
            float punto_aux[k];
            for (int i = 0; i < k; i++)
            {
                punto_aux[i] = true_son->point[i];
            }

            Node *aux = findParent(root, punto_aux);
            if (aux->left == true_son)
                aux->left = nullptr;
            else
                aux->right = nullptr;
        }
    }
    else if (current->left == nullptr)
    {
        true_son = find_min(current->right, 0);
        hijos_subarbol = getSubtreeWithoutNode(current->right, true_son);
        if (true_son)
        {
            // float punto_aux[11] = {true_son->point[0], true_son->point[1], true_son->point[2], true_son->point[3], true_son->point[4], true_son->point[5], true_son->point[6], true_son->point[7], true_son->point[8], true_son->point[9], true_son->point[10]};
            float punto_aux[k];
            for (int i = 0; i < k; i++)
            {
                punto_aux[i] = true_son->point[i];
            }
            Node *aux = findParent(root, punto_aux);
            if (aux->left == true_son)
                aux->left = nullptr;
            else
                aux->right = nullptr;
        }
    }

    else if (current->left != nullptr && current->right != nullptr)
    {
        true_son = find_min(current->left, 0);
        hijos_subarbol = getSubtreeWithoutNode(current->left, true_son);
        hijos_subarbol_dos = getSubtreeWithoutNode(current->right, true_son);
        hijos_subarbol.insert(hijos_subarbol.end(), hijos_subarbol_dos.begin(), hijos_subarbol_dos.end());
        if (true_son)
        {
            float punto_aux[k];
            for (int i = 0; i < k; i++)
            {
                punto_aux[i] = true_son->point[i];
            }
            Node *aux = findParent(root, punto_aux);
            if (aux->left == true_son)
            {
                aux->left = nullptr;
            }
            else
            {
                aux->right = nullptr;
            }
        }
    }

    if (parent == nullptr)
    {
        // El nodo a eliminar es la raíz
        if (true_son != nullptr)
        {
            // Hay un hijo, entonces actualiza la raíz
            root = true_son;
            for (auto i : hijos_subarbol)
                insert_per(true_son, i->point, dim);
        }
        else
        {
            // No hay hijos, simplemente elimina la raíz
            delete root;
            return nullptr;
        }
    }
    else
    {
        // No es la raíz
        if (parent->left == current)
        {
            // Estamos en el lado izquierdo del padre
            if (true_son != nullptr)
            {
                parent->left = true_son;
                for (auto i : hijos_subarbol)
                    insert_per(true_son, i->point, dim);
            }
            else
            {
                // No hay hijos, simplemente elimina el nodo actual
                delete current;
                parent->left = nullptr;
            }
        }
        else
        {
            // Estamos en el lado derecho del padre
            if (true_son != nullptr)
            {
                parent->right = true_son;
                for (auto i : hijos_subarbol)
                    insert_per(true_son, i->point, dim);
            }

            else
            {
                // No hay hijos, simplemente elimina el nodo actual
                delete current;
                parent->right = nullptr;
            }
        }
    }

    return root;
}

double calculateDistance(float point1[], float point2[])
{
    double distance = 0;
    for (int i = 0; i < k; ++i)
        distance += pow(point1[i] - point2[i], 2);

    return sqrt(distance);
}

double searchRec(Node *root, float point[], unsigned depth)
{
    if (root == NULL)
        return DBL_MAX; // Return a large value for points not found

    if (arePointsSame(root->point, point))
        return 0; // Distance is zero for identical points

    unsigned cd = depth % k;
    double distance = calculateDistance(root->point, point);

    if (point[cd] < root->point[cd])
        return min(distance, searchRec(root->left, point, depth + 1));

    return min(distance, searchRec(root->right, point, depth + 1));
}

double search(Node *root, float point[])
{
    return searchRec(root, point, 0);
}

void rangeQueryRec(Node *root, int dimension, float low, float high, unsigned depth)
{
    if (root == nullptr)
        return;

    unsigned cd = depth % k;

    // Verificar si el nodo actual está dentro del rango en la dimensión especificada
    if (root->point[dimension] >= low && root->point[dimension] <= high)
    {
        // Imprimir el punto
        cout << "Punto: [";
        for (int i = 0; i < k; ++i)
        {
            cout << root->point[i];
            if (i < k - 1)
                cout << ", ";
        }
        cout << "]" << endl;
    }

    // Verificar si el rango se superpone con el lado izquierdo del árbol
    if (low <= root->point[dimension])
        rangeQueryRec(root->left, dimension, low, high, depth + 1);

    // Verificar si el rango se superpone con el lado derecho del árbol
    if (high >= root->point[dimension])
        rangeQueryRec(root->right, dimension, low, high, depth + 1);
}

void rangeQuery(Node *root, int dimension, float low, float high)
{
    rangeQueryRec(root, dimension, low, high, 0);
}

int main(int argc, char *argv[])
{

    struct Node *root = NULL;

    ifstream file(argv[1]);
    if (!file.is_open())
    {
        cout << "File not found" << endl;
        return 1;
    }

    string line;
    int n = 0;

    while (getline(file, line))
    {
        n++;
    }

    file.clear();
    file.seekg(0, ios::beg);

    float points[n][k];

    int c = 0;
    while (getline(file, line))
    {
        points[c][0] = stof(line.substr(0, line.find(",")));
        line.erase(0, line.find(",") + 1);
        points[c][1] = stof(line.substr(0, line.find(",")));
        line.erase(0, line.find(",") + 1);
        points[c][2] = stof(line.substr(0, line.find(",")));
        line.erase(0, line.find(",") + 1);
        points[c][3] = stof(line.substr(0, line.find(",")));
        line.erase(0, line.find(",") + 1);
        points[c][4] = stof(line.substr(0, line.find(",")));
        line.erase(0, line.find(",") + 1);
        points[c][5] = stof(line.substr(0, line.find(",")));
        line.erase(0, line.find(",") + 1);
        points[c][6] = stof(line.substr(0, line.find(",")));
        line.erase(0, line.find(",") + 1);
        points[c][7] = stof(line.substr(0, line.find(",")));
        line.erase(0, line.find(",") + 1);
        points[c][8] = stof(line.substr(0, line.find(",")));
        line.erase(0, line.find(",") + 1);
        points[c][9] = stof(line.substr(0, line.find(",")));
        line.erase(0, line.find(",") + 1);
        points[c][10] = stof(line.substr(0, line.find(",")));
        c++;
    }

    fstream fout;
    fout.open("output.txt", ios::out);

    for (int i = 0; i < n; i++)
    {
        root = insert(root, points[i]);
    }

    int s = 0;
    s = 0;
    for (int i = 0; i < n; i++)
        if (search_node(root, points[i]))
            s++;
    cout << "S: " << s << endl;

    ////////////////////////
    //////// delete ////////
    ////////////////////////

    rangeQuery(root, 1, -0.5, 0.5);

    return 0;
}
