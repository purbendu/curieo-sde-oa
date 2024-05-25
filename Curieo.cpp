#include<bits/stdc++.h>
using namespace std;
typedef long double ld;
typedef long long ll;
#define all(x) begin(x),end(x)

class ErrorLog {
    private:
        long long timestamp;
        string logType;
        long double logSeverity; 

    public:
        ErrorLog(long long timestamp, string logType, long double logSeverity) {
            this->timestamp = timestamp;
            this->logType = timestamp;
            this->logSeverity = logSeverity;
        }
};
class SGTree {
	vector<ld> seg1, seg2, seg3;
public:
	SGTree(ll n) {
		seg1.resize(4 * n + 1);
        seg2.resize(4 * n + 1);
        seg3.resize(4 * n + 1);
	}

	void build1(ll ind, ll low, ll high, vector<ld> &arr) {
		if (low == high) {
			seg1[ind] = arr[low];
			return;
		}

		ll mid = (low + high)/2;

		build1(2 * ind + 1, low, mid, arr);
		build1(2 * ind + 2, mid + 1, high, arr);

        seg1[ind] = min(seg1[2 * ind + 1],seg1[2 * ind + 2]);
	}

	ld query1(ll ind, ll low, ll high, ll l, ll r) {
		if (r < low || high < l) return LDBL_MAX;

		if (low >= l && high <= r) return seg1[ind];

		ll mid = (low + high)/2;

		ld left = query1(2 * ind + 1, low, mid, l, r);
		ld right = query1(2 * ind + 2, mid + 1, high, l, r);

		return min(left, right);
	}

    void build2(ll ind, ll low, ll high, vector<ld> &arr) {
        if (low == high) {
            seg2[ind] = arr[low];
            return;
        }

        ll mid = (low + high)/2;

        build2(2 * ind + 1, low, mid, arr);
        build2(2 * ind + 2, mid + 1, high, arr);

        seg2[ind] = max(seg2[2 * ind + 1],seg2[2 * ind + 2]);
    }

    ld query2(ll ind, ll low, ll high, ll l, ll r) {
        if (r < low || high < l) return 0.0;

        if (low >= l && high <= r) return seg2[ind];

        ll mid = (low + high)/2;

        ld left = query2(2 * ind + 1, low, mid, l, r);
        ld right = query2(2 * ind + 2, mid + 1, high, l, r);

        return max(left, right);
    }

    void build3(ll ind, ll low, ll high, vector<ld> &arr) {
        if (low == high) {
            seg3[ind] = arr[low];
            return;
        }

        ll mid = (low + high)/2;

        build3(2 * ind + 1, low, mid, arr);
        build3(2 * ind + 2, mid + 1, high, arr);

        seg3[ind] = seg3[2 * ind + 1]+seg3[2 * ind + 2];
    }

    ld query3(ll ind, ll low, ll high, ll l, ll r) {
        if (r < low || high < l) return 0.0;

        if (low >= l && high <= r) return seg3[ind];

        ll mid = (low + high)/2;

        ld left = query3(2 * ind + 1, low, mid, l, r);
        ld right = query3(2 * ind + 2, mid + 1, high, l, r);

        return left+right;
    }
};
string splitter(string &s, ll &i) {
    string word = "";
    while(i < s.size() and s[i] != ' ') {
        word += s[i++];
    }
    i++;
    return word;
}
int main() {
    #ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    #endif

    vector<ErrorLog*> entry;
    map<long long,vector<long double>> prefTimestamp; // timestamp -> {min,max,sum,count} 
    map<long long,vector<long double>> suffTimestamp; // timestamp -> {min,max,sum,count} 

    map<string,vector<ld>> logTypeSeverities;

    map<ll,vector<ll>> timestampIndex;

    vector<string> operationsOrder;

    vector<ld> logSeverities;

    map<string,map<ll,vector<ll>>> logTypeTimestampIdx; // logType -> {timestamp, index in logTypeSeverities}

    int operationType;
    ll hsfbejhs = 0;
    while (cin>>operationType) {
        if(operationType == 1) {
            string operations;
            getline(cin, operations);
            stringstream split(operations);

            string timestampStr, logType, logSeverityStr;
            getline(split,timestampStr, ';');
            getline(split,logType, ';');
            getline(split,logSeverityStr, ';');

            long long timestamp;
            long double logSeverity; 
            timestamp = stoll(timestampStr);
            logSeverity = stold(logSeverityStr);

            ErrorLog *error = new ErrorLog(timestamp,logType,logSeverity);
            entry.push_back(error);

            logSeverities.push_back(logSeverity);

            logTypeSeverities[logType].push_back(logSeverity);

            logTypeTimestampIdx[logType][timestamp].push_back(logTypeSeverities[logType].size());
            timestampIndex[timestamp].push_back(entry.size());

            operations = "1 " + logType;
            operationsOrder.push_back(operations);
        }
        else if(operationType ==  2) {
            string logType;
            cin>>logType;
            logType = "2 " + logType;
            operationsOrder.push_back(logType);
        }
        else if(operationType == 3) {
            string operation;
            getline(cin, operation);

            operation = "3" + operation;
            operationsOrder.push_back(operation);
        }
        else if(operationType == 4) {
            string operation;
            getline(cin, operation);

            operation = "4" + operation;
            operationsOrder.push_back(operation);
        }
    }
    ll n = logSeverities.size();

    SGTree t(n);
    t.build1(0,0,n-1,logSeverities);
    t.build2(0,0,n-1,logSeverities);
    t.build3(0,0,n-1,logSeverities);
    map<string,SGTree*> logTypeSegTree;
    for(auto &logTypeSeverity:logTypeSeverities) {
        auto logType = logTypeSeverity.first;
        auto severity = logTypeSeverity.second;
        ll m = severity.size();
       
        logTypeSegTree[logType] = new SGTree(m);
        logTypeSegTree[logType]->build1(0,0,m-1,severity);
        logTypeSegTree[logType]->build2(0,0,m-1,severity);
        logTypeSegTree[logType]->build3(0,0,m-1,severity);
    }

    ll count = 0;
    map<string,ll> counter;


    for(string operation:operationsOrder) {
        if(operation[0] == '1') {
            count++;
            string logType;
            ll i = 2;
            logType = splitter(operation,i);
            counter[logType]++;
            cout<<"No output\n";
        }
        else if(operation[0] == '2') {
            ll i = 2;
            string logType = splitter(operation,i);
            ll m = logTypeSeverities[logType].size();


            ld minimum = logTypeSegTree[logType]->query1(0,0,m-1,0,counter[logType]-1);
            ld maximum = logTypeSegTree[logType]->query2(0,0,m-1,0,counter[logType]-1);
            ld sum = logTypeSegTree[logType]->query3(0,0,m-1,0,counter[logType]-1);

            cout<<"Min: "<<minimum<<" Max: "<<maximum<<" Mean: "<<(sum/counter[logType])<<endl;
        }
        else if(operation[0] == '3') {
            ll i = 2;

            string type = splitter(operation,i);
            ll timestamp = stoll(splitter(operation,i));

            if(type == "BEFORE") {
                timestamp -= 2;
                auto k = timestampIndex.upper_bound(timestamp);
                
                if(k == timestampIndex.end()) {
                    ld minimum = t.query1(0,0,n-1,0,count-1);
                    ld maximum = t.query2(0,0,n-1,0,count-1);
                    ld sum = t.query3(0,0,n-1,0,count-1);

                    cout<<"Min: "<<minimum<<" Max: "<<maximum<<" Mean: "<<(sum/count)<<endl;
                    continue;
                }
                if(k == timestampIndex.begin()) {
                    if((*k).first > timestamp+1) {
                        cout<<"Min: 0 Max: 0 Mean: 0\n";
                        continue;
                    }
                }
                auto j = (*k).second;

                ll idx = *lower_bound(all(j),count-1);
                ld minimum = t.query1(0,0,n-1,0,idx);
                ld maximum = t.query2(0,0,n-1,0,idx);
                ld sum = t.query3(0,0,n-1,0,idx);

                cout<<"Min: "<<minimum<<" Max: "<<maximum<<" Mean: "<<(sum/(idx+1))<<endl;

            }
            else {
                timestamp++;
                auto k = timestampIndex.lower_bound(timestamp);
                if(k == timestampIndex.end()) {
                    cout<<"Min: 0 Max: 0 Mean: 0\n";
                    continue;
                }
                auto j = (*k).second;

                ll idx = *lower_bound(all(j),count-1);
                ld minimum = t.query1(0,0,n-1,idx-1,count-1);
                ld maximum = t.query2(0,0,n-1,idx-1,count-1);
                ld sum = t.query3(0,0,n-1,idx-1,count-1);

                cout<<"Min: "<<minimum<<" Max: "<<maximum<<" Mean: "<<(sum/(count-idx+1))<<endl;
            }
        }
        else {
            ll i = 2;

            string type = splitter(operation,i);
            string logType = splitter(operation,i);
            ll timestamp = stoll(splitter(operation,i));

            if(type == "BEFORE") {
                timestamp -= 2;

                auto k = logTypeTimestampIdx[logType].upper_bound(timestamp);
                ll m = logTypeSeverities[logType].size();
                
                if(k == logTypeTimestampIdx[logType].end()) {
                    ld minimum = logTypeSegTree[logType]->query1(0,0,m-1,0,counter[logType]-1);
                    ld maximum = logTypeSegTree[logType]->query2(0,0,m-1,0,counter[logType]-1);
                    ld sum = logTypeSegTree[logType]->query3(0,0,m-1,0,counter[logType]-1);

                    cout<<"Min: "<<minimum<<" Max: "<<maximum<<" Mean: "<<(sum/(counter[logType]))<<endl;
                    continue;
                }
                if(k == logTypeTimestampIdx[logType].begin()) {
                    if((*k).first > timestamp+1) {
                        cout<<"Min: 0 Max: 0 Mean: 0\n";
                        continue;
                    }
                }
                auto j = (*k).second;
                ll idx = *lower_bound(all(j),counter[logType]-1);
                
                ld minimum = logTypeSegTree[logType]->query1(0,0,m-1,0,idx);
                ld maximum = logTypeSegTree[logType]->query2(0,0,m-1,0,idx);
                ld sum = logTypeSegTree[logType]->query3(0,0,m-1,0,idx);

                cout<<"Min: "<<minimum<<" Max: "<<maximum<<" Mean: "<<(sum/(idx+1))<<endl;
            }
            else {
                timestamp++;

                auto k = logTypeTimestampIdx[logType].lower_bound(timestamp);
                if(k == logTypeTimestampIdx[logType].end()) {
                    cout<<"Min: 0 Max: 0 Mean: 0\n";
                    continue;
                }
                auto j = (*k).second;

                ll idx = *lower_bound(all(j),counter[logType]-1);

                ll m = logTypeSeverities[logType].size();
                ld minimum = logTypeSegTree[logType]->query1(0,0,m-1,idx,counter[logType]-1);
                ld maximum = logTypeSegTree[logType]->query2(0,0,m-1,idx,counter[logType]-1);
                ld sum = logTypeSegTree[logType]->query3(0,0,m-1,idx,counter[logType]-1);

                cout<<"Min: "<<minimum<<" Max: "<<maximum<<" Mean: "<<(sum/(counter[logType]-idx))<<endl;
            }

        }
    }
}