//temporary file for testing of database
#ifndef TEST_DATABASE_H
#define TEST_DATABASE_H

#include "databasememory.h"
#include "databasedisk.h"

#include <iostream>
#include <string>

using namespace std;

void test(DatabaseInterface& dbi) {

	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;

	string title1 = "title1";
	string title2 = "title2";
	string title3 = "title3";

	dbi.insertNewsgroup(title1);
	dbi.insertNewsgroup(title2);
	dbi.insertNewsgroup(title2);
	dbi.insertNewsgroup(title3);
	dbi.removeNewsgroup(c);

	cout << "Newsgroups:" << endl;
	for (auto i : dbi.listNewsgroups()) {
		cout << "id: " << i.first << " title: " << i.second << endl;
	}

	cout << endl;

	string atitle1 = "atitle1";
	string atitle2 = "atitle2";
	string atitle3 = "atitle3";
	string atitle4 = "atitle4";

	string author1 = "author1";
	string author2 = "author2";
	string author3 = "author3";
	string author4 = "author4";


	string text1 = "text1";
	string text2 = "text2";


	dbi.insertArticle(a, atitle1, author1, text1);
	dbi.insertArticle(a, atitle2, author2, text2);
	dbi.insertArticle(a, atitle3, author3, text2);
	dbi.insertArticle(a, atitle4, author4, text2);

	dbi.removeArticle(a, b);


	cout << "Articles in Newsgroup " << a << ":" << endl;
	vector<pair<int, string>> v;
	dbi.listArticles(a, v);

	for (auto i : v) {
		cout << "id: " << i.first << " title: " << i.second << endl;
	}

	cout << endl << "Get Article: " << endl;

	string getTitle;
	string getAuthor;
	string getText;

	dbi.getArticle(a, d, getTitle, getAuthor, getText);

	cout << "id: " << b << " Title: " << getTitle << " Author: " << getAuthor << " Text: " << getText << endl << endl;
}

int main() {

	cout << "---TEST MEMORY DATABASE---" << endl << endl;

	DatabaseMemory dbmem;
	test(dbmem);

	cout << "---TEST DISK DATABASE---" << endl << endl;

	DatabaseDisk dbdisk;
	test(dbdisk);
}
#endif