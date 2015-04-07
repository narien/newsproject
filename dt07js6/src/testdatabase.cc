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
	string title4 = "title4 spacetest";

	dbi.insertNewsgroup(title1);
	dbi.insertNewsgroup(title2);
	dbi.insertNewsgroup(title2);
	dbi.insertNewsgroup(title3);

	dbi.removeNewsgroup(c);

	dbi.insertNewsgroup(title4);

	cout << "Newsgroups:" << endl;
	for (auto i : dbi.listNewsgroups()) {
		cout << "id: " << i.first << " title: " << i.second << endl;
	}

	cout << endl;

	string atitle1 = "atitle1";
	string atitle2 = "atitle2";
	string atitle3 = "atitle3";
	string atitle4 = "atitle4 spacetest";

	string author1 = "author1";
	string author2 = "author2";
	string author3 = "author3";
	string author4 = "author4 spacetest";


	string text1 = "text1";
	string text2 = "text2";
	string text_multiline = "text1\ntext2\ntext3";


	dbi.insertArticle(a, atitle1, author1, text1);
	dbi.insertArticle(a, atitle2, author2, text2);
	dbi.insertArticle(a, atitle3, author3, text2);
	dbi.insertArticle(a, atitle4, author4, text_multiline);

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

	string getTitle1;
	string getAuthor1;
	string getText1;

	dbi.getArticle(a, d, getTitle, getAuthor, getText);
	dbi.getArticle(a, a, getTitle1, getAuthor1, getText1);

	cout << "id: " << d << " Title: " << getTitle << " Author: " << getAuthor << " Text: " << endl << getText << endl;
	cout << "id: " << d << " Title: " << getTitle1 << " Author: " << getAuthor1 << " Text: " << endl << getText1 << endl << endl;
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