/*
 * m_sql_parser.cpp
 *
 *  Created on: 2014-8-30
 *      Author: casa
 */

#include "m_sql_parser.h"

SqlParser::SqlParser() {
	// TODO Auto-generated constructor stub

}

SqlParser::~SqlParser() {
	// TODO Auto-generated destructor stub
}

void Sqlparser::execute(){
	int count=1;
	while(1){
		string tablename;
		Node* oldnode=getparsetreeroot();
		timeval finish_parser_time;
		gettimeofday(&finish_parser_time, NULL);
		cout<<"parser use "<<(double)(finish_parser_time.tv_usec - start_time.tv_usec)/1000+(finish_parser_time.tv_sec - start_time.tv_sec)<<" ms"<<endl;
		if(oldnode == NULL){
			printf("[ERROR]there are some wrong in statement! please try again!!\n");
			FreeAllNode();
			continue;
		}
		Stmt *stmtList = (Stmt *)oldnode;
		while (stmtList != NULL){
			Node *node = (Node *)stmtList->data;
			switch(node->type){
			case t_query_stmt:{
				SQLParse_log("this is query stmt!!!!!!!!!!!!!!!!!!");
				if (!semantic_analysis(node,false))//---3.22fzh---
					SQLParse_elog("semantic_analysis error");
				preprocess(node);
					Query_stmt *querynode=(Query_stmt *)node;
					if(querynode->from_list!=NULL)
					int fg=solve_join_condition(querynode->from_list);
					if(querynode->where_list!=NULL)
					{
						struct Where_list * curt=(struct Where_list *)(querynode->where_list);
						struct Node *cur=(struct Node *)(curt->next);
						SQLParse_log("wc2tb");
						departwc(cur,querynode->from_list);
					}
				LogicalOperator* plan=parsetree2logicalplan(node);//现在由于没有投影，所以只把from_list传输进去。因此在完善之后，需要在parsetree2logicalplan()中
				//进行判断，对于不同的语句，比如select,update等选择不同的操作。
				LogicalOperator* root=NULL;
				if(querynode->limit_list!=NULL)
				{
					Limit_expr *lexpr=(Limit_expr *)querynode->limit_list;
					if(lexpr->offset==NULL)
					{
						root=new LogicalQueryPlanRoot(0,plan,LogicalQueryPlanRoot::PRINT,LimitConstraint(atoi(((Expr *)lexpr->row_count)->data)));
					}
					else
					{
						root=new LogicalQueryPlanRoot(0,plan,LogicalQueryPlanRoot::PRINT,LimitConstraint(atoi(((Expr *)lexpr->row_count)->data),atoi(((Expr *)lexpr->offset)->data)));
					}
				}
				else
				{
					root=new LogicalQueryPlanRoot(0,plan,LogicalQueryPlanRoot::PRINT);
				}

				BlockStreamIteratorBase* physical_iterator_tree=root->getIteratorTree(64*1024);
				cout<<"~~~~~~~~~physical plan~~~~~~~~~~~~~~"<<endl;
				physical_iterator_tree->print();
				cout<<"~~~~~~~~~physical plan~~~~~~~~~~~~~~"<<endl;

				physical_iterator_tree->open();
				while(physical_iterator_tree->next(0));
				physical_iterator_tree->close();
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
