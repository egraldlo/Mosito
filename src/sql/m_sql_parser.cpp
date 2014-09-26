/*
 * m_sql_parser.cpp
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#include "m_sql_parser.h"

SqlParser::SqlParser() {

}

SqlParser::~SqlParser() {

}

void SqlParser::execute() {
	int count=1;
	while(1){
		/* getparsetreeroot function return a Node* linklist. */
		Node* oldnode=getparsetreeroot();

		gettimeofday(&finish_time_, NULL);

		cout<<"sql parser used time: ";
		cout<<(double)(finish_time_.tv_usec-start_time_.tv_usec)/1000+(finish_time_.tv_sec-start_time_.tv_sec);
		cout<<" ms"<<endl;

		if(oldnode==0){
			cout<<"some error occurred in your statement!!!"<<endl;
			//FreeAllNode();
			continue;
		}

		Stmt *stmtList = (Stmt *)oldnode;

		while (stmtList != NULL){
			Node *node = (Node *)stmtList->data;
			switch(node->type){
			case t_query_stmt:{
				cout<<"Query>>"<<endl;
//				if (!semantic_analysis(node,false))//---3.22fzh---
//					SQLParse_elog("semantic_analysis error");
//				preprocess(node);
//					Query_stmt *querynode=(Query_stmt *)node;
//					if(querynode->from_list!=NULL)
//					int fg=solve_join_condition(querynode->from_list);
//					if(querynode->where_list!=NULL)
//					{
//						struct Where_list * curt=(struct Where_list *)(querynode->where_list);
//						struct Node *cur=(struct Node *)(curt->next);
////						departwc(cur,querynode->from_list);
//					}
					QueryPlan *plan=ast_2_lp(node);
//					QueryPlan *root=new physical::Debug(,plan);
			}
			break;
			default:
			{
				cout<<node->type<<endl;
				puts("nothing matched!\n");
			}
			}
			stmtList = (Stmt *)stmtList->next;
		}
	}
}

Node * SqlParser::getparsetreeroot(){
	int charnum=0;
//	globalReadOffset = 0;
//	NodePointer.clear();
//	errorNumber = 0;
	parsetreeroot = 0;
	memset(globalInputText, 0, sizeof(globalInputText));
	printf("Claims>");
	while(1){
		char c=getchar();
		globalInputText[charnum++]=c;
		if(c==';'){
			globalInputText[charnum++]='\0';
			break;
		}
	}
	gettimeofday(&start_time_, NULL);
	if(!yyparse()){
		return parsetreeroot;
	}
	else{
		printf("SQL parse failed\n");
		return NULL;
	}
}

QueryPlan *SqlParser::ast_2_lp(Node *tree) {
	QueryPlan *plan=0;
	//Query_stmt *node=(Query_stmt *)parsetree;
	//Query_stmt is a key datastructure.
	switch(tree->type) {
		case t_query_stmt: {
			plan=osgwf_2_lp(tree);
		}
		default: {
			cout<<"parser tree is null!"<<endl;
			plan=0;
		}
	}
	return plan;
}

QueryPlan *SqlParser::osgwf_2_lp(Node *tree) {
	QueryPlan *order=0;
	QueryPlan *sgwf=sgwf_2_lp(tree);
	Query_stmt *node=(Query_stmt *)tree;
	if(node->orderby_list==0) {
		order=sgwf;
	}
	else {
		/*
		 * TODO: add sort logical plan.
		 *  */
	}
	return order;
}


