#include <iostream>
#include <string>
#include <vector>
#include "ArgumentManager.h"
#include <fstream>
#include <stack>

using namespace std;

void preParse(string &s) {
	for (int i = 0; i< s.size() - 1; i++) {
		if (s[i] == '-' && s[i-1] == '(') {
			s.insert(i, "0");
		}
	}
}

bool isValid(string s) {
	stack<char> st;
	char x;

	for (char c : s) {
		if (c == '(' || c == '[' || c == '{') {
			st.push(c);
			continue;
		}

		switch (c) {
			case ')':
				x = st.top();
				st.pop();
				if (x == '{' || x == '[') return false;
				break;
			case ']':
				x = st.top();
				st.pop();
				if (x == '(' || x == '{') return false;
				break;
			case '}':
				x = st.top();
				st.pop();
				if (x == '(' || x == '[') return false;
				break;
		}
	}
	return (st.empty());
}

int prec(char op){
    if(op == '+'||op == '-') return 1;
    if(op == '*'||op == '/') return 2;
    return 0;
}

string inToPost(string s) {
	stack<char> st;
	string result;
	for (char c : s) {
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c>= '0' && c <= '9')) {
			result+=c;
		} else if (c == '(' || c == '{' || c == '[') {
			st.push(c);
		} else if (c == ')') {
			while (st.top() != '(') {
				result+=st.top();
				st.pop();
			}
			st.pop();
		} else if (c == '}') {
			while (st.top() != '{') {
				result+=st.top();
				st.pop();
			}
			st.pop();
		} else if (c == ']') {
			while (st.top() != '[') {
				result+=st.top();
				st.pop();
			}
			st.pop();
		} else {
			while (!st.empty() && prec(c) <= prec(st.top())) {
				result+=st.top();
				st.pop();
			}
			st.push(c);
		}
	}

	while (!st.empty()) {
		result+=st.top();
		st.pop();
	}
	return result;
}

int postFixEval(string s) {
	stack<int> st;
	for (char c : s) {
		if (isdigit(c)) {
			st.push(c-'0');
		} else if (isalpha(c)) {
			st.push(int(c));
		} else {
			int val1 = st.top();
			st.pop();
			if (st.empty()) {
				switch(c) {
					case '+': st.push(val1); break;
					case '-': st.push(-val1); break;
				}
				break;
			}
			int val2 = st.top();
			st.pop();
			switch (c) {
				case '+': st.push(val2+val1); break;
				case '-': st.push(val2-val1); break;
				case '*': st.push(val1*val2); break;
				case '/': st.push(val2/val1); break;
			}
		}
	}
	return st.top();
}


bool sameAnswers(vector<string> expressions) {

	for (int i = 1; i < expressions.size(); i++) {
		if (i == 1) preParse(expressions[0]);
		preParse(expressions[i]);
		if (postFixEval(inToPost(expressions[i])) != postFixEval(inToPost(expressions[i-1]))) {
			return false;
		}
	}

	return true;
}


int main(int argc, char *argv[]) {
	ArgumentManager am(argc, argv);
	string infilename = am.get("input");
	string outfilename = am.get("output");
	ifstream ifs(infilename);
	ofstream ofs(outfilename);
	
	string currExpression;
	vector<string> expressions;
	bool hasErrors = false;

	while (getline(ifs, currExpression)) {
		if (currExpression.length() == 0) {
			continue;
		}
		expressions.push_back(currExpression);
	}
	for (string s : expressions) {
		cout << s <<  ": " << isValid(s) << endl;
		if (!isValid(s)) {
			hasErrors=true;
		}
	}


	if (hasErrors) {
		for (int i = 0; i < expressions.size(); i++) {
			if (!isValid(expressions[i])) {
				ofs << "Error at: " << i + 1 << endl;
			}
		}
	} else {
		if (sameAnswers(expressions)) {
			ofs << "Yes";
		} else {
			ofs << "No";
		}
	}


	
	return 0;
}

