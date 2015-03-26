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
	string text1 = "text1";
	string text2 = "text2";


	cout << dbmem.insertArticle(a, "atitle1", "author1", text1);
	dbmem.insertArticle(a, "atitle2", "author2", text2);

	cout << "Articles in Newsgroup 1:" << endl;
	vector<pair<int, string>> v;
	dbmem.listArticles(a, v);
	for (auto i : v) {
		cout << "id: " << i.first << " title: " << i.second << endl;
	}

	cout << "TEST DISK DATABASE" << endl << endl;

	DatabaseDisk dbdisk;
}
#endif