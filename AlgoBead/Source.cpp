#include <vector>
#include <iostream>
#include <string>
#include <fstream> 

using namespace std;

class Vertex;
void DFS(Vertex*u);

vector<Vertex> vertices;
int numberOfVertices;

string start_vertex_name;
string destini_vertex_name;

int depthN=0;
int traversalN=0;
bool DAG=true; 

class Edge
{
public:
    Edge(Vertex *org, Vertex *dest, int dist)
    {
        origin = org;
        destination = dest;
        distance = dist;
    }

    Vertex* getOrigin() {return origin;}
    Vertex* getDestination() {return destination;}
    int getDistance() {return distance;}
private:
    Vertex* origin;
    Vertex* destination;
    int distance;
};


class Vertex
{
public:
	Vertex() {}
    Vertex(string id)
    {
        name = id;
		color=false;
		max_dist=-1;
		parent=nullptr;

		depthNumber=0;
		traversalNumber=0;
		depthColor=0;
		deepParent=nullptr;
    }

    void addEdge(Vertex *v, int dist)
    {
        Edge newEdge(this, v, dist);
        edges.push_back(newEdge);
    }

    void printEdges()
    {
        cout << name << ":" << endl;
        for (int i = 0; i < edges.size(); i++)
        {
        Edge e = edges[i];
        cout << e.getDestination()->getName() <<
            " - " << e.getDistance() << endl;
        }
        cout << endl;
    }

    string getName() {return name;} 
	int getMax_dist() {return max_dist;}
	Vertex* getParent() {return parent;}
	void setMax_dist(int n) {max_dist=n;}
	void setParent(Vertex*v){parent=v;}
    vector<Edge> getEdges() {return edges;}

	int getDepthNumber() {return depthNumber;}
	int getTraversalNumber() {return traversalNumber;}
	int getDepthColor() {return depthColor;}
	void setDepthNumber(int n) {depthNumber=n;}
	void setTraversalNumber(int n){traversalNumber=n;}
	void setDepthColorToGrey(){depthColor=1;}
	void setDepthColorToBlack(){depthColor=2;}
	void setDepthParent(Vertex *v){ deepParent=v;}

private:
	string name;
    vector<Edge> edges;
	int max_dist;
	Vertex*parent;

	int depthNumber;
	int traversalNumber;
	int depthColor; //0-white, 1-grey, 2-black
	Vertex*deepParent;

public:
	bool color;
};

void DepthFirstSearch()
{
	for(int i=0; i<numberOfVertices; i++)
	{
		if(vertices[i].getDepthColor()==0)
		{
			DFS(&vertices[i]);
		}
	}
}

void DFS(Vertex*u)
{
	u->setDepthColorToGrey();
	++depthN;
	u->setDepthNumber(depthN);
	for(int i=0; i<(u->getEdges()).size(); i++)
	{
		Vertex*v=u->getEdges()[i].getDestination();
		if(v->getDepthColor()==0)
		{
			v->setDepthParent(u);
			DFS(v);
		}
		else if((v->getDepthNumber())<=(u->getDepthNumber()) && v->getTraversalNumber()==0)
		{
			DAG=false;
		}

	}
	u->setDepthColorToBlack();
	++traversalN;
	u->setTraversalNumber(traversalN);
}



int condMaxSearch()							
{
	int max=-1;
	int temp=-1;
	for(int i=0; i<numberOfVertices; i++)
	{
		if(vertices[i].color == false && vertices[i].getMax_dist()>max)
		{
			max=vertices[i].getMax_dist();
			temp=i;
		}
	}
	return temp;
}

int findVertex(string name)				
{
	int number;
	for(int i=0; i<numberOfVertices; i++)
	{
		if(vertices[i].getName()==name)
		{
			number=i;
		}
	}
	return number;
}

void dijsktra()
{
	int foundVertex=findVertex(start_vertex_name);

	vertices[foundVertex].setMax_dist(0); 

	for(int i=0; i<numberOfVertices; i++)
	{
		int indOfMax=condMaxSearch();
		if(indOfMax==-1) return;

		Vertex *u = &vertices[indOfMax];
		u->color=true;							
		
		for(int j=0; j<(u->getEdges()).size(); j++)
		{
			Vertex* temp=u->getEdges()[j].getDestination();
			int indFoundVetrtex=findVertex(temp->getName());
			if(vertices[indFoundVetrtex].getMax_dist()<(vertices[indOfMax].getMax_dist()+u->getEdges()[j].getDistance()))
			{
				vertices[indFoundVetrtex].setMax_dist(vertices[indOfMax].getMax_dist()+u->getEdges()[j].getDistance());
				vertices[indFoundVetrtex].setParent(u);
			}
		}
	}
}

vector<string> split(string str, string delim)
	{ 
	unsigned start = 0;
	unsigned end; 
	vector<string> v; 

	while( (end = str.find(delim, start)) != string::npos )
	{ 
		v.push_back(str.substr(start, end-start)); 
		start = end + delim.length(); 
	} 
	v.push_back(str.substr(start)); 
	return v; 
}

void read(string s)
{
	
	vector<string> splitLine;
	string sLine = "";
	string name;
      ifstream infile;
	  int line=1;
	  string path;
	  switch (stoi(s))
	  {
		case 1: path="test1.txt";
			break;
		case 2: path="test2.txt";
			break;
		case 3: path="test3.txt";
			break;
	  }
      

      infile.open(path.c_str());
      while (!infile.eof())
      {
           getline(infile, sLine);
		   if(line==1)
		   {
			   numberOfVertices=stoi( sLine );
		   }
		   else if(line==2)
		   {
			   splitLine=split(sLine, " "); // here it is

				for(int i=0; i<numberOfVertices; i++)
				{
					vertices.push_back(Vertex(splitLine[i]));
				}
		   }
		   else if(line>2 && line<=(2+numberOfVertices))
		   {
			   splitLine=split(sLine, " "); 

			   for(int i=0; i<numberOfVertices; i++)
			   {
				   int temp=stoi(splitLine[i]);
				   if(temp>0)
				   {
					   vertices[line-3].addEdge(&vertices[i], temp);
				   }
			   }
		   }
		   else if(line==numberOfVertices+3)
		   {
			   start_vertex_name=sLine;
		   }
		   else
		   {
			   destini_vertex_name=sLine;
		   }
		   line++;
      }
      infile.close();

}

void running()
{
	cout<<"Melyik tesztet szerete futtatni?"<<endl;
	cout<<endl;
	cout<<"1 - Iranyitott kormentes graf, start es vegpont kozott van ut"<<endl;
	cout<<"2 - Iranyitott kormentes graf, start es vegpont kozott nincs ut"<<endl;
	cout<<"3 - Iranyitott nem kormentes graf" <<endl;
	cout<<endl;
	cout<<"Kerem valassszon egy szamot: ";
	bool ok=false;
	string mystring="";
	while(ok==false)
	{
		cin>>mystring;
		if(mystring=="1" || mystring=="2" ||mystring=="3") ok=true;
		else
		{
			cout<<"A kovetezo szamok egyiket adja meg: 1, 2, 3."<<endl;
			cout<<"A választott szam: ";
		}
	}
	cout<<endl;
	cout<<"-----------------------------------------------------------------"<<endl;
	cout<<endl;
	read(mystring);
	DepthFirstSearch();
	if(DAG==true)
	{
		dijsktra();

		bool l=false;
		for(int i=0; i<numberOfVertices; i++)
		{
			if(vertices[i].getName()==destini_vertex_name && vertices[i].getMax_dist()>0)
			{
				cout<<start_vertex_name<<" csucsbol "<<vertices[i].getName()<<" csucsba "<<vertices[i].getMax_dist()<<" hosszu a maximalis ut."<<endl;
				l=true;

				vector<string> way;
				Vertex *p=&vertices[i];
				while(p!=nullptr)
				{
					way.push_back(p->getName());
					p=p->getParent();
				}
				int size=way.size();
				cout<<"Az utvonal: ";
				for(int j=0; j<size; j++)
				{
					if(j!=size-1) cout<<way.back()<<" -> ";
					else cout<<way.back();
					way.pop_back();
				}
				cout<<endl;
			}
	
		}

		if(l==false)
		{
			cout<<"Ezek kozott a csucsok kozott nincsen ut!"<<endl;
		}
	}
	else
	{
		cout<<"Nem kormentes a graf!"<<endl;
	}
}

int main()
{
	running();

	cout<<endl;
	cout<<"-----------------------------------------------------------------"<<endl;
	cout<<endl;
	
	system("PAUSE");
	
	return 0;
}