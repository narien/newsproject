//Interface to database, intended to be implemented by memory and disk databases

#ifndef DATABASE_INTERFACE_H
#define DATABASE_INTERFACE_H

#include <string>
#include <vector>
#include <utility>

using namespace std;

class DatabaseInterface {
	public:
		virtual ~DatabaseInterface() {}; //have to provide virtual destructor implementation
		virtual bool insertNewsgroup(const string& title) = 0; //=0 subclasses have to implement (pure virtual)
		virtual bool insertArticle(const int& newsgroup_id, const string& article_title, const string& article_author, const string& article_text) = 0;
		virtual bool removeNewsgroup(const int& newsgroup_id) = 0;
		virtual int removeArticle(const int& newsgroup_id, const int& article_id) = 0; //returns 1 if it worked, 0 if no such newsgroup and -1 if no such article
    	virtual vector<pair<int, string>> listNewsgroups() const = 0; //<newsgroup_id, newsgroup_title>
    	virtual bool listArticles(const int& newsgroup_id, vector<pair<int, string>>& articles) const = 0; //articles -> <article_id, article_title>
    	virtual int getArticle(const int& newsgroup_id, const int& article_id, string& article_title, string& article_author, string& article_text) const = 0; //returns 1 if it worked, 0 if no such newsgroup and -1 if no such article, writes the relevant data to the input variables
};
#endif