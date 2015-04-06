//Database in memory

#ifndef DATABASE_MEMORY_H
#define DATABASE_MEMORY_H

#include "databaseinterface.h"
#include "newsgroup.h"

#include <vector>
#include <string>

using namespace std;

class DatabaseMemory : public DatabaseInterface {
	public:
		DatabaseMemory();
		~DatabaseMemory();
		bool insertNewsgroup(const string& title) override;
		bool insertArticle(const int& newsgroup_id, const string& article_title, const string& article_author, const string& article_text) override;
		bool removeNewsgroup(const int& newsgroup_id) override;
		int removeArticle(const int& newsgroup_id, const int& article_id) override;
		vector<pair<int, string>> listNewsgroups() const override;
    	bool listArticles(const int& newsgroup_id, vector<pair<int, string>>& articles) const override;
    	int getArticle(const int& newsgroup_id, const int& article_id, string& article_title, string& article_author, string& article_text) const override;

	private:
		int newsgroup_cntr;
		vector<Newsgroup> newsgroups;
};
#endif