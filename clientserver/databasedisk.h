//Database on disk

#ifndef DATABASE_DISK_H
#define DATABASE_DISK_H

#include "databaseinterface.h"
#include "newsgroup.h"

#include <string>

using namespace std;

class DatabaseDisk : public DatabaseInterface {
	public:
		DatabaseDisk();
		~DatabaseDisk();
		bool insertNewsgroup(string title) override;
		bool insertArticle(int& newsgroup_id, string& article_title, string& article_author, string& article_text) override;
		bool removeNewsgroup(int& newsgroup_id) override;
		int removeArticle(int& newsgroup_id, int& article_id) override;
		vector<pair<int, string>> listNewsgroups() override;
    	bool listArticles(int& newsgroup_id, vector<pair<int, string>>& articles) override;
    	int getArticle(const int& newsgroup_id, const int& article_id, string& article_title, string& article_author, string& article_text) override;

	private:
		int newsgrp_cntr;
		const string path = "./db/";
};
#endif