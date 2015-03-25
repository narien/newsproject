//Interface to database, intended to be implemented by memory and disk databases

#ifndef DATABASE_INTERFACE_H
#define DATABASE_INTERFACE_H

#include "newsgroup.h"

#include <string>

using namespace std;

class DatabaseInterface {
	public:
		virtual bool insert_newsgroup(string& title);
		virtual bool insert_article(int& newsgroup_id, string& article_title, string& article_author, string& article_text);
		virtual bool remove_newsgroup(int& newsgroup_id);
		virtual bool remove_article(int& newsgroup_id, int& article_id);
};
#endif