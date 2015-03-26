//structs for internal database representation

#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <string>
#include <vector>

using namespace std;

struct Article {

		int id;
		string title;
		string author;
		string text;
};

struct Newsgroup {

		int id;
		int article_cntr = 1;
		string title;
		vector<Article> articles;
};
#endif