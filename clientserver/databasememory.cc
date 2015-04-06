//Database in memory

#include "databasememory.h"

#include <algorithm>


DatabaseMemory::DatabaseMemory() {

	newsgroup_cntr = 1;
}

DatabaseMemory::~DatabaseMemory() {

}

bool DatabaseMemory::insertNewsgroup(const string& title) {

	//check if already exist
	auto it = find_if(newsgroups.begin(), newsgroups.end(), [title] (const Newsgroup& ng) { return ng.title == title; });

	if (!title.empty() && it == newsgroups.end()) {

		Newsgroup tmp_grp = Newsgroup();
		tmp_grp.title = title;
		tmp_grp.id = newsgroup_cntr;
		newsgroup_cntr++;
		newsgroups.push_back(tmp_grp);
		return true;
	}

	else {
		return false;
	}
}

bool DatabaseMemory::insertArticle(const int& newsgroup_id, const string& article_title, const string& article_author, const string& article_text) {

	//find newsgroup
	auto it = find_if(newsgroups.begin(), newsgroups.end(), [newsgroup_id] (const Newsgroup& ng) { return ng.id == newsgroup_id; });

	if (!article_title.empty() && it != newsgroups.end()) {

		Article tmp_article = {it->article_cntr, article_title, article_author, article_text};
		it->article_cntr++;

		it->articles.push_back(tmp_article);
		return true;
	}

	else {
		return false;
	}
}

bool DatabaseMemory::removeNewsgroup(const int& newsgroup_id) {

	//find newsgroup
	auto it = find_if(newsgroups.begin(), newsgroups.end(), [newsgroup_id] (const Newsgroup& ng) { return ng.id == newsgroup_id; });

	if(it != newsgroups.end()) {
		newsgroups.erase(it);
        return true;
	}
	else {
		return false;
	}
}

//returns 1 if it worked, 0 if no such newsgroup and -1 if no such article
int DatabaseMemory::removeArticle(const int& newsgroup_id, const int& article_id) {

	//find newsgroup
	auto itn = find_if(newsgroups.begin(), newsgroups.end(), [newsgroup_id] (const Newsgroup& ng) { return ng.id == newsgroup_id; });

	if(itn != newsgroups.end()) {

		//find article
		auto ita = find_if(itn->articles.begin(), itn->articles.end(), [article_id] (const Article& a) { return a.id == article_id; });

        if(ita != itn->articles.end()) {
        	itn->articles.erase(ita);
            return 1;
        } else {
            return -1;
        }
	}
	else {
		return 0;
	}
}

vector<pair<int, string>> DatabaseMemory::listNewsgroups() const {

	vector<pair<int, string>> v;

	for (auto i : newsgroups) {
		v.push_back(make_pair(i.id, i.title));
	}
	return v;
}

//returns 1 if it worked, 0 if no such newsgroup
bool DatabaseMemory::listArticles(const int& newsgroup_id, vector<pair<int, string>>& articles) const {

	//find newsgroup
	auto itn = find_if(newsgroups.begin(), newsgroups.end(), [newsgroup_id] (const Newsgroup& ng) { return ng.id == newsgroup_id; });

	if(itn != newsgroups.end()) {

		for (auto i : itn->articles) {
			articles.push_back(make_pair(i.id, i.title));
		}
		return 1;
	}
	else {
		return 0;
	}
}

//returns 1 if it worked, 0 if no such newsgroup and -1 if no such article, writes the relevant data to the input variables
int DatabaseMemory::getArticle(const int& newsgroup_id, const int& article_id, string& article_title, string& article_author, string& article_text) const {
	
	//find newsgroup
	auto itn = find_if(newsgroups.begin(), newsgroups.end(), [newsgroup_id] (const Newsgroup& ng) { return ng.id == newsgroup_id; });

	if(itn != newsgroups.end()) {

		//find article
		auto ita = find_if(itn->articles.begin(), itn->articles.end(), [article_id] (const Article& a) { return a.id == article_id; });

        if(ita != itn->articles.end()) {
			article_title = ita->title;
			article_author = ita->author;
			article_text = ita->text;
            return 1;
        } else {
            return -1;
        }
	}
	else {
		return 0;
	}
}