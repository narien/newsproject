//Interface to database, intended to be implemented by memory and disk databases

#ifndef DATABASE_INTERFACE_H
#define DATABASE_INTERFACE_H

#include "newsgroup.h"
#include "protocol.h"

#include <string>
#include <utility>
using std::pair;

using namespace std;

class DatabaseInterface {
	public:
		virtual bool insert_newsgroup(string& title) = 0;
		virtual bool insert_article(int& newsgroup_id, string& article_title, string& article_author, string& article_text) = 0;
		virtual bool remove_newsgroup(int& newsgroup_id) = 0;
		virtual int remove_article(int& newsgroup_id, int& article_id) = 0; //returns 1 if it worked, 0 if no such newsgroup and -1 if no such article
    virtual vector<pair<int, string>> ListNewsgroups() = 0;
    virtual bool listArticles(int& newsgroup_id, vector<pair<int, string s>>& articles) = 0;
    virtual int get_article(const int&newsgroup_id, const int& article_id, string& article_title, string& article_author, string& article_text) = 0; //returns 1 if it worked, 0 if no such newsgroup and -1 if no such article, writes the relevant data to the input variables
};
#endif