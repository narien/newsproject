//Interface to database, intended to be implemented by memory and disk databases

//just a test

#ifndef DATABASE_INTERFACE_H
#define DATABASE_INTERFACE_H

#include "newsgroup.h"

#include <string>

using namespace std;

class DatabaseInterface {
	public:
		virtual bool insert_newsgroup(string& title) = 0;
		virtual bool insert_article(int& newsgroup_id, string& article_title, string& article_author, string& article_text) = 0;
		virtual bool remove_newsgroup(int& newsgroup_id) = 0;
		virtual bool remove_article(int& newsgroup_id, int& article_id) = 0;
    virtual vector<Newsgroup> ListNewsgroups() = 0;
    virtual vector<Article> listArticles(int& newsgroup_id) = 0;
};
#endif