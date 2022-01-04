#include<iostream>
#include<fstream>
#include<vector>
#include<string>

using namespace std;

//min value
#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

//Max value
#ifndef Max
#define Max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

//greatest common divisor
int gcd(int m, int n){
    while(n != 0){
	    
        int r = m % n; 
        m = n; 
        n = r; 
    } 
    return m;
}

//least common multiple(by gcd)
int lcm(int m, int n){
	
    return m * n / gcd(m, n);
}

struct Task{
    int TID;//ID of Task
	int Phase;//the first Job's Release time
	int Period;
	float WCET;//worst-case execution time
	int RDeadline; //relative deadline
	float Utilization;//=total clocks Jobs consume / total clocks
};

struct Job{
	int release_time;//=Period*n+Phase
	float remain_execution_time;//剩餘執行時間=WCET-已經執行的時間
	int absolute_deadline;//=release_time + RDeadline/Period
	int TID; //屬於哪個Task的工作
};

//檔案打開
const char * fileName ="test.txt";/*========================================開檔名稱========================================*/

void EDF(){
	
	//檔案打開(讀檔)
	ifstream fp_r;
	fp_r.open(fileName,ios::in);
	
	//檔案打開(寫檔)
	ofstream fp_EDF;
	fp_EDF.open("EDF.txt",ios::out);

	//相關參數
	int Total_Job_Number=0;
	int Miss_Deadline_Job_Number=0;
	int MaxPH=0;
	int LCM=1;
	int Clock=0;
	int Tid=1;// TID 從1開始
	float S_test=0;//Schedulability test

	Task T;
	
	vector<Task> L;
	vector<Task>::iterator it;
	vector<Job> Q;
	vector<Job>::iterator Q_it;
	
	//讀檔
	if(!fp_r)//如果開啟檔案失敗，fp_r為0；成功，fp_r為非0
		cout<<"Fail to open file: "<<fileName<<endl;
	else{
		string s;
		int j;
		
		while(getline(fp_r,s)){
			
			vector<string> buf(4);
			j=0;

			//過濾掉逗號跟空格
			for(int i=0; i<s.length(); i++){
				
				if(s.at(i)==','){
					
					j++;
				}
				else if(s.at(i)!=' '){
					
					buf[j].push_back(s.at(i));
				}
			}

			
			T.TID=Tid;
			T.Phase=stoi(buf[0]);
			T.Period=stoi(buf[1]);
			T.RDeadline=stoi(buf[2]);
			T.WCET=stof(buf[3]);
			L.push_back(T);
			Tid++;
			
		}
	}

	for (it=L.begin(); it!=L.end(); it++){
		
		S_test=S_test+((*it).WCET/min((*it).Period,(*it).RDeadline));//Schedulability test

		LCM = lcm((*it).Period, LCM);

		MaxPH = Max(MaxPH,(*it).Phase);	
	}
	
	//Schedulability test 沒過
	if(S_test>1){
		fp_EDF<<"It failed to pass Schedulability test"<<endl<<endl;
		cout<<"It failed to pass Schedulability test"<<endl<<endl;
	}

	fp_EDF<<"EDF"<<endl<<endl;
	cout<<"EDF"<<endl<<endl;

	while(Clock <= (LCM+MaxPH)){
		
		Job J;
		for (it=L.begin(); it!=L.end(); it++){
			
			//針對每一個任務判斷目前時間Clock是否為它的工作之抵達時間
			//CLK在Phase前不做事
			if(Clock>=(*it).Phase){
				if((Clock-(*it).Phase)%(*it).Period == 0){
					
					J.TID = (*it).TID;
					J.release_time = Clock;
					
					//absolute_deadline給定
					if((*it).Period <= (*it).RDeadline)					
						J.absolute_deadline = J.release_time+(*it).Period;
					else
						J.absolute_deadline = J.release_time+(*it).RDeadline;
					
					J.remain_execution_time = (*it).WCET;//初始執行時間
					
					Q.push_back(J);
					
					Total_Job_Number++;	  
				}
			}
		}    
		for(Q_it=Q.begin(); Q_it!=Q.end(); Q_it++){
			
			//判斷Q中的每一個工作是否能在它的絕對截限時間之前完成
			if((*Q_it).absolute_deadline-Clock-(*Q_it).remain_execution_time < 0){ 
				
				cout<<"Clock"<<Clock<<" T"<<(*Q_it).TID<<" MISS"<<endl;
				Q_it=Q.erase(Q_it);
				Miss_Deadline_Job_Number++;
			}
			else
				continue;
		}
		//輸出CLK編號
		fp_EDF<<Clock;
		cout<<Clock;
		
		int min_TID = 1000;
		int min_deadline = 1000;
		int priority_id_EDF;
	   
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		//EDF
		for(Q_it=Q.begin(); Q_it!=Q.end(); Q_it++){
			
			if((*Q_it).absolute_deadline == min_deadline){
				
				if((*Q_it).TID < min_TID){
					
					min_TID = (*Q_it).TID;
					priority_id_EDF = (*Q_it).TID;
				}
				
			}
			else if((*Q_it).absolute_deadline < min_deadline){
				
				priority_id_EDF=(*Q_it).TID;
				min_TID = (*Q_it).TID;
				min_deadline = (*Q_it).absolute_deadline;
			}
		}

		//將選好的Job運作
		for(Q_it=Q.begin(); Q_it!=Q.end(); Q_it++){
			
			if((*Q_it).TID == priority_id_EDF){
				
				fp_EDF<<" T";
				fp_EDF<<priority_id_EDF;
				cout<<" T";
				cout<<priority_id_EDF;

				(*Q_it).remain_execution_time--;
				if((*Q_it).remain_execution_time == 0){  
					
					Q_it=Q.erase(Q_it);
					break;
				}
			}
		}
		fp_EDF<<endl;
		cout<<endl;
		Clock++;
	}
	cout<<"Total Job Number: "<<Total_Job_Number<<endl;
	fp_EDF<<"Total Job Number: "<<Total_Job_Number<<endl;
	
	cout<<"Miss Deadline Job Number: "<<Miss_Deadline_Job_Number<<endl<<endl;
	fp_EDF<<"Miss Deadline Job Number: "<<Miss_Deadline_Job_Number<<endl<<endl;
	
	//關檔
	fp_EDF.close();
	fp_r.close();
}

void RM(){
	
	//檔案打開(讀檔)
	ifstream fp_r;
	fp_r.open(fileName,ios::in);
	
	//檔案打開(寫檔)
	ofstream fp_RM;
	fp_RM.open("RM.txt",ios::out);

	//相關參數
	int Total_Job_Number=0;
	int Miss_Deadline_Job_Number=0;
	int MaxPH=0;
	int LCM=1;
	int Clock=0;
	int Tid=1;// TID 從1開始

	Task T;
	
	vector<Task> L;
	vector<Task>::iterator it;
	vector<Job> Q;
	vector<Job>::iterator Q_it;
	
	//讀檔
	if(!fp_r)//如果開啟檔案失敗，fp_r為0；成功，fp_r為非0
		cout<<"Fail to open file: "<<fileName<<endl;
	else{
		string s;
		int j;
		
		while(getline(fp_r,s)){
			vector<string> buf(4);
			j=0;

			//過濾掉逗號跟空格
			for(int i=0; i<s.length(); i++){
				if(s.at(i)==','){
					j++;
				}
				else if(s.at(i)!=' '){
					buf[j].push_back(s.at(i));
				}
			}

			T.TID=Tid;
			T.Phase=stoi(buf[0]);
			T.Period=stoi(buf[1]);
			T.RDeadline=stoi(buf[2]);
			T.WCET=stof(buf[3]);
			L.push_back(T);
			Tid++;
			
		}
	}

	for (it=L.begin(); it!=L.end(); it++){

		LCM = lcm((*it).Period, LCM);

		MaxPH = Max(MaxPH,(*it).Phase);	
	}

	fp_RM<<"RM"<<endl<<endl;
	cout<<"RM"<<endl<<endl;

	while(Clock <= (LCM+MaxPH)){
		Job J;
		for (it=L.begin(); it!=L.end(); it++){
			//針對每一個任務判斷目前時間Clock是否為它的工作之抵達時間
			//CLK在Phase前不做事
			if(Clock>=(*it).Phase){
				if((Clock-(*it).Phase)%(*it).Period == 0){
					J.TID = (*it).TID;
					J.release_time = Clock;
					
					//absolute_deadline給定
					if((*it).Period <= (*it).RDeadline)					
						J.absolute_deadline = J.release_time+(*it).Period;
					else
						J.absolute_deadline = J.release_time+(*it).RDeadline;
					
					J.remain_execution_time = (*it).WCET;//初始執行時間
					
					Q.push_back(J);
					
					Total_Job_Number++;	  
				}
			}
		}    
		for(Q_it=Q.begin(); Q_it!=Q.end(); Q_it++){
			//判斷Q中的每一個工作是否能在它的絕對截限時間之前完成
			if((*Q_it).absolute_deadline-Clock-(*Q_it).remain_execution_time < 0){ 
				cout<<"Clock"<<Clock<<" T"<<(*Q_it).TID<<" MISS"<<endl;
				Q_it=Q.erase(Q_it);
				Miss_Deadline_Job_Number++;
			}
			else
				break;
		}
		//輸出CLK編號
		fp_RM<<Clock;
		cout<<Clock;

		int min_period = 1000;
		int priority_id_RM;
	   
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		//RM 排程
		for(Q_it=Q.begin(); Q_it!=Q.end(); Q_it++){
			for (it=L.begin(); it!=L.end(); it++){
				if((*it).TID==(*Q_it).TID){
					if((*it).Period < min_period){
						min_period = (*it).Period;
						priority_id_RM = (*it).TID;
					}
				}
			}
		}
		//將選好的Job運作
		for(Q_it=Q.begin(); Q_it!=Q.end(); Q_it++){
			if((*Q_it).TID == priority_id_RM){
				fp_RM<<" T";
				fp_RM<<priority_id_RM;
				cout<<" T";
				cout<<priority_id_RM;

				(*Q_it).remain_execution_time--;	

				if((*Q_it).remain_execution_time == 0){   
					Q_it=Q.erase(Q_it);
					break;
				}
			}	   
		}

		fp_RM<<endl;
		cout<<endl;
		Clock++;
	}
	cout<<"Total Job Number: "<<Total_Job_Number<<endl;
	fp_RM<<"Total Job Number: "<<Total_Job_Number<<endl;    
	
	cout<<"Miss Deadline Job Number: "<<Miss_Deadline_Job_Number<<endl<<endl;
	fp_RM<<"Miss Deadline Job Number: "<<Miss_Deadline_Job_Number<<endl<<endl;
	
	//關檔
	fp_RM.close();
	fp_r.close();
}

int main(){
	
	EDF();
	RM();
	
	return 0;
}
