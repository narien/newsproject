//Database in memory

#ifndef DATABASEMEM_H
#define DATABASEMEM_H

#include "databaseinterface.h"
#include "newsgroup.h"

#include <map>
#include <string>

using namespace std;

class DatabaseMemory : DatabaseInterface {
	public:
		DatabaseMemory();
		~DatabaseMemory();
		bool insertNewsgroup(string& title) override;
		bool insertArticle(int& newsgroup_id, string& article_title, string& article_author, string& article_text) override;
		bool removeNewsgroup(int& newsgroup_id) override;
		int removeArticle(int& newsgroup_id, int& article_id) override;

	private:
		int newsgrp_cntr; //unique id number
		map<int, Newsgroup> newsgroups;
};

#endif