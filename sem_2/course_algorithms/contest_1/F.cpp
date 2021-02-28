#include <iostream>
#include <vector>
#include <algorithm>
 
using namespace std;
 
vector<int> get_ans(const vector<vector<int>>& dp, const vector<int>& s1, const vector<int>& s2, int i, int j, int l) {
  vector<int> ret;
  while (ret.size() < l) {
    if (s1[i - 1] == s2[j - 1]) {
      ret.push_back(s2[j - 1]);
      --i;
      --j;
    } else {
      if (dp[i - 1][j] >= dp[i][j - 1]) {
        --i;
      } else {
        --j;
      }
    }
  }
  reverse(ret.begin(), ret.end());
  return ret;
}
 
vector<int> max_shared_part(vector<int> first, vector<int> second) {
  int l1 = first.size();
  int l2 = second.size();
  vector<vector<int>> dp(l1 + 1, vector<int>(l2 + 1, 0));
  for (int i = 1; i <= l1; ++i) {
    for (int j = 1; j <= l2; ++j) {
      if (first[i - 1] == second[j - 1]) {
        dp[i][j] = dp[i - 1][j - 1] + 1;
      }
      dp[i][j] = max(dp[i][j], dp[i][j - 1]);
      dp[i][j] = max(dp[i][j], dp[i - 1][j]);
    }
  }
 
  vector<int> ans = get_ans(dp, first, second, l1, l2, dp[dp.size() - 1][dp[0].size() - 1]);
  return ans;
}
 
int main() {
  int n = 0;
  scanf("%d", &n);
  vector<int> first(n);
  for (int i = 0; i < n; ++i) {
    scanf("%d", &first[i]);
  }
  scanf("%d", &n);
  vector<int> second(n);
  for (int i = 0; i < n; ++i) {
    scanf("%d", &second[i]);
  }
  vector<int> ret = max_shared_part(first, second);
  printf("%zu\n", ret.size());
 
  return 0;
}
