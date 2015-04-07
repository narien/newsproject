//Database on disk

#ifndef DATABASE_DISK_H
#define DATABASE_DISK_H

#include "databaseinterface.h"

#include <string>
#include <vector>

using namespace std;

class DatabaseDisk : public DatabaseInterface {
	public:
		DatabaseDisk();
		~DatabaseDisk();
		bool insertNewsgroup(const string& title) override;
		bool insertArticle(const int& newsgroup_id, const string& article_title, const string& article_author, const string& article_text) override;
		bool removeNewsgroup(const int& newsgroup_id) override;
		int removeArticle(const int& newsgroup_id, const int& article_id) override;
		vector<pair<int, string>> listNewsgroups() const override;
    	bool listArticles(const int& newsgroup_id, vector<pair<int, string>>& articles) const override;
    	int getArticle(const int& newsgroup_id, const int& article_id, string& article_title, string& article_author, string& article_text) const override;

	private:
		int readNewsgroupCntr() const;
		int readArticleCntr(const int& newsgroup_id) const;
		const string path = "./db/";
		const unsigned char isDir = 0x4;
		const unsigned char isFile = 0x8;
};
#endif