//structs for internal database representation

#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <string>
#include <map>

using namespace std;

struct Article {

		string title;
		string author;
		string text;
};

struct Newsgroup {

		int article_cntr = 1;
		string title;
		map<int, Article> articles;
};

#endif