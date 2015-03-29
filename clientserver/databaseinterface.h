//Interface to database, intended to be implemented by memory and disk databases

#ifndef DATABASE_INTERFACE_H
#define DATABASE_INTERFACE_H

#include "newsgroup.h"

#include <string>
#include <vector>
#include <utility>

using namespace std;

class DatabaseInterface {
	public:
		virtual bool insertNewsgroup(string title) = 0; //=0 subclasses have to implement (pure virtual)
		virtual bool insertArticle(int& newsgroup_id, string& article_title, string& article_author, string& article_text) = 0;
		virtual bool removeNewsgroup(int& newsgroup_id) = 0;
		virtual int removeArticle(int& newsgroup_id, int& article_id) = 0; //returns 1 if it worked, 0 if no such newsgroup and -1 if no such article
    	virtual vector<pair<int, string>> listNewsgroups() = 0; //<newsgroup_id, newsgroup_title>
    	virtual bool listArticles(int& newsgroup_id, vector<pair<int, string>>& articles) = 0; //articles -> <article_id, article_title>
    	virtual int getArticle(const int& newsgroup_id, const int& article_id, string& article_title, string& article_author, string& article_text) = 0; //returns 1 if it worked, 0 if no such newsgroup and -1 if no such article, writes the relevant data to the input variables
};
#endif