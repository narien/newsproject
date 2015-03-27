//temporary file for testing of database
#ifndef TEST_DATABASE_H
#define TEST_DATABASE_H

#include "databasememory.h"
#include "databasedisk.h"

#include <iostream>
#include <string>

using namespace std;

int main() {

	cout << "TEST MEMORY DATABASE" << endl << endl;

	DatabaseMemory dbmem;
	dbmem.insertNewsgroup("title1");
	dbmem.insertNewsgroup("title2");
	dbmem.insertNewsgroup("title2");
	dbmem.insertNewsgroup("title3");
	int i = 3;
	dbmem.removeNewsgroup(i);

	cout << "Newsgroups:" << endl;
	for (auto i : dbmem.listNewsgroups()) {
		cout << "id: " << i.first << " title: " << i.second << endl;
	}

	cout << endl;

	int a = 1;
	int b = 2;
	string text1 = "text1";
	string text2 = "text2";


	dbmem.insertArticle(a, "atitle1", "author1", text1);
	dbmem.insertArticle(a, "atitle2", "author2", text2);
	dbmem.insertArticle(a, "atitle3", "author3", text2);
	dbmem.insertArticle(a, "atitle4", "author4", text2);

	dbmem.removeArticle(a, b);


	cout << "Articles in Newsgroup " << a << ":" << endl;
	vector<pair<int, string>> v;
	dbmem.listArticles(a, v);
	for (auto i : v) {
		cout << "id: " << i.first << " title: " << i.second << endl;
	}

	cout << endl << "Get Article: " <<endl;

	string getTitle;
	string getAuthor;
	string getText;

	int c = 4;

	dbmem.getArticle(a, c, getTitle, getAuthor, getText);

	cout << "id: " << b << " Title: " << getTitle << " Author: " << getAuthor << " Text: " << getText << endl;

	cout << endl;

	cout << "TEST DISK DATABASE" << endl << endl;

	DatabaseDisk dbdisk;
}
#endif