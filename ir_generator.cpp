#include <iostream>
#include <vector>
#include <string>
using namespace std;

/* --------- AST Node (Tree) ----------- */
struct ASTNode {
    string value;
    ASTNode* left;
    ASTNode* right;

    ASTNode(string val) {
        value = val;
        left = right = NULL;
    }
};

/* --------- Three Address Code (TAC) Structure ----------- */
struct TAC {
    string result;
    string arg1;
    string op;
    string arg2;

    TAC(string r, string a1, string o, string a2) {
        result = r;
        arg1 = a1;
        op = o;
        arg2 = a2;
    }

    void print() {
        if(op == "=")
            cout << result << " = " << arg1 << endl;
        else
            cout << result << " = " << arg1 << " " << op << " " << arg2 << endl;
    }
};

/* --------- IR Generator ----------- */
class IRGenerator {
    int tempCount = 1;

public:
    vector<TAC> instructions;

    string newTemp() {
        return "t" + to_string(tempCount++);
    }

    string generate(ASTNode* root) {
        if (!root) return "";

        // If leaf node (variable or number)
        if (root->left == NULL && root->right == NULL) {
            return root->value;
        }

        // Post-order traversal
        string left = generate(root->left);
        string right = generate(root->right);

        string temp = newTemp();

        instructions.push_back(TAC(temp, left, root->value, right));

        return temp;
    }

    void printTAC() {
        for (auto &ins : instructions) {
            ins.print();
        }
    }
};

/* --------- MAIN FUNCTION ----------- */
int main() {

    // AST for: a = b + c * d

    ASTNode* root = new ASTNode("=");

    root->left = new ASTNode("a");

    ASTNode* plus = new ASTNode("+");
    root->right = plus;

    plus->left = new ASTNode("b");

    ASTNode* mul = new ASTNode("*");
    plus->right = mul;

    mul->left = new ASTNode("c");
    mul->right = new ASTNode("d");

    IRGenerator ir;

    string finalResult = ir.generate(root);

    // Assignment: a = t?
    ir.instructions.push_back(TAC("a", finalResult, "=", ""));

    cout << "\n--- Three Address Code ---\n";
    ir.printTAC();

    return 0;
}
