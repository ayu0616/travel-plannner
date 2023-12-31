// スコアの定義を指定してみる
#include <bits/stdc++.h>
using namespace std;

#include "../tp-lib.hpp"

int N;
vector<Place> places;

struct State : public StateBase {
    // スコアは高いほうが良い
    float score() const override {
        float s = 1 - (float)arrive_at / places[0].arrive_before;  // 到着時間が早いほど良い
        rep(i, N) {
            if (visited >> i & 1) s += places[i].priority;
        }
        return s;
    }

    // ゴール判定
    bool is_goal() const { return visited == (1 << N) - 1; }
};

struct compare_state {
    bool operator()(const State *s, const State *t) const { return s->score() < t->score(); }
};

using state_pq = priority_queue<State *, vector<State *>, compare_state>;

int main() {
    time_t start = time(nullptr);
    cin >> N;
    vvi A(N, vi(N));
    rep(i, N) rep(j, N) cin >> A[i][j];

    places.resize(N);
    rep(i, N) {
        places[i].id = i;
        int ab, aa, st, pr;
        cin >> ab >> aa >> st >> pr;
        if (ab != -1) places[i].arrive_before = ab;
        if (aa != -1) places[i].arrive_after = aa;
        places[i].stay_time = st;
        places[i].priority = pr;
    }

    // 初期状態
    State *init = new State();
    init->place = &places[0];
    init->prev = nullptr;
    init->arrive_at = 0;
    init->visited = 0;

    state_pq que;  // スコアが高い順に取り出す
    que.push(init);

    state_pq answers;  // 解となる状態を格納する
    while (!que.empty()) {
        State *s = que.top();
        que.pop();
        Place cp = *s->place;
        // 全スポットを訪れたら終了
        if (s->is_goal()) {
            if (s->is_end) continue;
            s->is_end = true;
            answers.push(s);
            continue;
        }
        // 現在の場所で滞在してから0に戻ると間に合わない場合
        if (s->arrive_at + cp.stay_time + A[cp.id][0] > places[0].arrive_before) {
            auto tmp = new State();
            auto prev = s->prev;
            if (prev->is_end) continue;
            prev->is_end = true;
            tmp->place = &places[0];  // 最初の場所に戻る
            tmp->prev = prev;
            tmp->arrive_at = prev->arrive_at + prev->place->stay_time + A[prev->place->id][0];
            tmp->visited = (s->visited ^ (1 << cp.id)) | (1 << 0);
            tmp->is_end = true;
            answers.push(tmp);
            continue;
        }

        rep(i, N) {
            // 訪れたことがある場所はスキップ
            if (s->visited >> i & 1 || (i == 0 && s->visited_len() != N - 1)) continue;
            int arrive_at = s->arrive_at + cp.stay_time + A[cp.id][i];  // 滞在時間と移動時間を足す
            // 到着時間が制約を満たさない場合はスキップ
            if (!places[i].is_visitable(arrive_at)) continue;
            State *next = new State();
            next->place = &places[i];
            next->prev = s;
            next->arrive_at = arrive_at;
            next->visited = s->visited | (1 << i);
            que.push(next);
        }
    }

    // if (!answers.empty()) {
    //     auto ans = answers.top();
    //     auto visited_places = ans->trace_back();

    //     cout << ans->arrive_at << endl;
    //     cout << visited_places << endl;
    //     cerr << "time: " << time(nullptr) - start << "s" << endl;
    // } else {
    //     cout << "-1" << endl;
    // }

    // for (int i = 0; i < 10 && !answers.empty(); i++) {
    //     auto ans = answers.top();
    //     answers.pop();
    //     auto visited_places = ans->trace_back();
    //     vi ids;
    //     for (auto p : visited_places) ids.push_back(p->place->id);
    //     cout << ans->score() << endl;
    //     cout << ids << endl;
    // }
    auto ans = answers.top();
    auto visited_places = ans->trace_back();
    vi ids;
    for (auto p : visited_places) ids.push_back(p->place->id);
    cout << ans->score() << endl;
    cout << ids << endl;
}
