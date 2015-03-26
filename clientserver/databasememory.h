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
		bool insert_newsgroup(string& title) override;
		bool insert_article(int& newsgroup_id, string& article_title, string& article_author, string& article_text) override;
		bool remove_newsgroup(int& newsgroup_id) override;
		int remove_article(int& newsgroup_id, int& article_id) override;

	private:
		int newsgrp_cntr; //unique id number
		map<int, Newsgroup> newsgroups;
};

#endif