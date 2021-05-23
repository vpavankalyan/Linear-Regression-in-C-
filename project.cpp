#include<bits/stdc++.h>
using namespace std;
class csv_read{
	protected:
	vector <pair<string,vector<float> > >data;
	vector <float>x,y,yp,y_gra,x_test,y_test;
	string file_name;
	float x_factor,y_factor;
	int n;
	
	public:
		void read(string file)
		{
			file_name = file;
			cout<<"read csv file: "<<file_name<<endl;
			fstream fo;
			fo.open(file_name.c_str(), ios::in);
			string line,colname;
			if(fo.good()){
				getline(fo,line);
				stringstream s(line);
				while(getline(s,colname,',')){
				//	cout<<colname<<endl;
					data.push_back({colname,vector<float> {}});
				}
				int temp1;
				while(getline(fo,line)){
					int col=0;
					stringstream s1(line);
					while(s1 >> temp1){
					//	cout<<"value "<<x<<endl;
						data.at(col).second.push_back(temp1);
						col++;
					//	cout<<s1.peek()<<endl;
						if(s1.peek() == ','){
							s1.ignore();
						}
					}
				}
				for(int i=0;i<data[0].second.size();i++){
					x.push_back(data[0].second.at(i));
				}
				for(int i=0;i<data[1].second.size();i++){
					y.push_back(data[1].second.at(i));
				}
			}
			else{
				cout<<"error in file";
			}
			fo.close();
			check();
			data_normalization();
			spilt_data();
			
		}
		void check(){
			int i=0;
			while(i<data[0].second.size()){
				if(x.at(i)!=data[0].second.at(i)){
					cout<<"error in x"<<endl;
					break;
				}
			i++;
			}
			i=0;
			while(i<data[1].second.size()){
				if(y.at(i)!=data[1].second.at(i)){
					cout<<"error in y"<<endl;
					break;
				}
			i++;
			}
		}
    void set_size()
    {
        n=x.size();
    }
    void printdata()
    {
        for(int i=0;i<n;i++)
        {
            cout<<x[i]<<" "<<y[i]<<endl;
        }
    }
    
    void data_normalization(int normal_level = 7, int type =0)
    {
    	long double x_sum=0, y_sum=0;
    	
    	for(int i=0; i<x.size();i++)
    	{
    		x_sum += x[i];
    		y_sum += y[i];
		}
		
		x_factor = x_sum / (normal_level*x.size());
		y_factor = y_sum / (normal_level*y.size());
	//	cout<<"x-factor "<<x_factor<<endl<<" y-factor"<<y_factor<<endl;
		
		for(int i=0; i<x.size();i++)
    	{
    		x[i] = x[i] / x_factor;
    		y[i] = y[i] / y_factor;
		}
		
	}
    
    void spilt_data(int debug=0)
    {
    	int total = 0.3 * x.size();
    	srand(time(0));
    	
    	if(debug)
    	{
    		cout<<"index values"<<endl;
		}
    	for (int i=0; i<total; i++)
    	{
    		int x_mod = x.size();
	    	int y_mod = y.size();
	    	 
	    	int temp = rand();
	    	int x_index = temp%x_mod;
	    	int y_index = temp%y_mod;
	    	if(debug)
			{
				cout<<x_index<<endl;
			}
			
	    	x_test.push_back(x[x_index]);
			y_test.push_back(y[y_index]);
			
			x.erase(x.begin() + x_index);
			y.erase(y.begin() + y_index);
		}
		if(debug)
		{
			cout<<"X_test"<<endl;
		}
		
		if(debug)
		{
			for (int i=0; i < x_test.size(); i++)
	    	{
	    		cout<<x_test[i]<<endl;
	    	}
		}
    	
    	cout<<endl;
	}
};
class LinearRegression:public csv_read
{
protected:
    float m,c,xbar,ybar;
    float slope,intercept;
public:
    LinearRegression()
    {
        m=0;
        c=0;
        xbar=0;
        ybar=0;
    }
    void calculate()
    {
       float z,q,s=0,d=0;
	   float siz=y.size();
       xbar=accumulate(x.begin(),x.end(),0)/siz;
       ybar=accumulate(y.begin(),y.end(),0)/siz;
       for(int i=0;i<n;i++)
       {
           z=(x[i]-xbar);
           q=(y[i]-ybar);
           s=s+(z*q);
           d=d+z*z;
       }
       
        m=(s/d);
        c=ybar-m*xbar;
    }
    void show()
    {
        cout<<"Slope of the line by closed form eqn: "<<m<<endl;
        cout<<"Intercept of the line by closed form eqn: "<<c<<endl;
    }
    void predicted()
    {
        int i;
        for(i=0;i<y_test.size();i++)
       {
          float z;
          z=m*x_test[i]+c;
          yp.push_back(z);
          //cout<<z*x_factor<<"real :"<<y_test[i]*y_factor<<endl;
       }
       
    }
    
    void gradient_descent()
    {
        vector<float>error;             // array to store all error values
        float err = 0;
        float b0 = 0;                   //initializing b0
        float b1 = 0;                   //initializing b1
        float alpha = 0.00019;             //intializing error rate
        int max_learning_loops = 4700;
	    
	    for (int j=0; j < max_learning_loops; j++)
	    {
	    	for (int i = 0; i < n; i ++)
		    {
			    float p = b0 + b1 * x[i];
			    err += (p - y[i])* (p - y[i]);
		    }
		    
		    error.push_back(err);
			
		    b0 = b0 - (alpha * err);
		    float err1 = 0;
			
			for (int i = 0; i < n; i ++)
		    {
			    float p = b0 + b1 * x[i];
			    err1 += (p - y[i])* (p - y[i]);
		    }
		    
		    b0 = b0 + (2 * alpha * err);
		    float err2 = 0;
		    
		    for (int i = 0; i < n; i ++)
		    {
			    float p = b0 + b1 * x[i];
			    err2 += (p - y[i])* (p - y[i]);
		    }
		    
		    if(err1<err2)
		    {
		    	b0 = b0 - (2 * alpha * err);
			}
			
			b1 = b1 - (alpha * err);
		    err1 = 0;
		    
		    for (int i = 0; i < n; i ++)
		    {
			    float p = b0 + b1 * x[i];
			    err1 += (p - y[i])* (p - y[i]);
		    }
		    
		    b1 = b1 + (2*alpha * err);
		    err2 = 0;
		    
		    for (int i = 0; i < n; i ++)
		    {
			    float p = b0 + b1 * x[i];
			    err2 += (p - y[i])* (p - y[i]);
		    }
		    
		    if(err1 < err2)
		    {
		    	b1 = b1 - (2 * alpha * err);
			}
			
			err=0;
			
		//	cout<<"B0-value: "<<b0<<" "<<"B1-value: "<<b1<<endl;
		}
		
	    //sort(error.begin(),error.end(),custom_sort);//sorting based on error values
	    cout<<"Final Values by geadient descent are: "<<"c= "<<b0<<" "<<"m= "<<b1<<" "<<endl;
	    slope=b1;
	    intercept=b0;
    }
    void predict_gradient()
    {
        for(int i=0;i<x_test.size();i++)
        {
            y_gra.push_back((x_test[i] * slope) + intercept);
        }
    }
};
class Accuracy:public LinearRegression
{
protected:
    float r2f,r2g,r;
public:
    Accuracy()
    {
        r2f=0;
        r=0;
        r2g=0;
    }
    void correlation()
    {
       float z,q,s=0,d=0,siz=n,b=0,sq;
       for(int i=0;i<y.size();i++)
       {
           z=(x[i]-xbar);
           q=(y[i]-ybar);
           s=s+(z*q);
           d=d+z*z;
           b+=(q*q);
       }
       sq=sqrtf(d*b);
       r=(s/sq);
       cout<<"Correlation= "<<r<<endl;
       if(r>-0.5 && r<-1.0)
       {
           cout<<"Strong Negative relationship."<<endl;
           cout<<"Slope is negative."<<endl;
       }
       else if(r>=-0.5 && r<=0.5)
       {
           cout<<"No strong relationship between data."<<endl;
           cout<<"Data is not good for linear regression."<<endl;
       }
       else if(r>0.5 && r<=1.0)
       {
           cout<<"Strong Positive relationship."<<endl;
           cout<<"Slope is positive."<<endl;
       }
       cout<<endl;
    }
    void rsquare_for_formula()
    {
    	float ytbar=accumulate(y_test.begin(),y_test.end(),0)/y_test.size();
        float z=0,s=0;
        for(int i=0;i<y_test.size();i++)
        {
              z+=((yp[i]-y_test[i])*(yp[i]-y_test[i]));
              s+=((y[i]-ytbar)*(y[i]-ytbar));
        }
        r2f=1-(z/s);
        cout<<"R squared for closed form  :"<<r2f<<endl;
    }
    void rsquare_for_gradient()
    {
        float numg,deng=0;
        float y_t_bar=accumulate(y_test.begin(),y_test.end(),0)/y_test.size();
        for(int i=0; i<x_test.size(); i++)
        {
            float temp = (x_test[i] * slope) + intercept;
          //  cout<<"Predict: "<<temp*y_factor<<" Actual: "<<y_test[i]*y_factor<<endl;
            numg += ((temp-y_test[i]) * (temp-y_test[i]));
            deng+=((y_test[i]-y_t_bar)*(y_test[i]-y_t_bar));
        }
        cout<<"R2 for Gradient descent model: "<<1-(numg/deng)<<endl<<endl;
    }
};

int main()
{
    Accuracy a;
    a.read("statistics.csv");
    a.set_size();
    a.gradient_descent();
    a.predict_gradient();
    a.rsquare_for_gradient();
    a.correlation();
    a.calculate();
    a.show();
    a.predicted();
    a.rsquare_for_formula();
}
