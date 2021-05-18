#include "Position.h"
bool moveNode::operator==(moveNode& node1) {
    return this->src == node1.src && this->dst == node1.dst &&
           this->iseat == node1.iseat;
}
//����
bool readyForKill(int sdPlayer, const int* ucsqPieces,
                  vector<vector<int>> possible) {
    for (int i = 0 + sdPlayer * 16; i < 16 + sdPlayer * 16; i++) {
        for (int j = 0; j < possible[i].size(); j++) {
            if (possible[i][j] == ucsqPieces[!sdPlayer * 16]) return true;
        }
    }
    return false;
}

//׽ hunterΪ����������λ�ã�targetΪ���ط�����λ��
bool readyForCatch(int hunter, int target, const int* ucsqPieces,
                   vector<vector<int>> possible) {
    int pos = 0;
    for (int i = 0; i < 32; i++) {
        if (ucsqPieces[i] == hunter) {
            pos = i;
            break;
        }
    }
    for (int i = 0; i < possible[pos].size(); i++)
        if (possible[pos][i] == target) return true;
    return false;
}

int PositionStruct::Repeat() {
    //�췽�����򷵻�1���췽�������ڷ������򷵻�2���ڷ�������˫��������/�ظ������кͷ���0��ƽ�֣���û���ظ������򷵻�-1
    //����ܻ��õ��ĳ�Ա������Count,sdPlayer,Moves

    if (Count < 12) return -1;
    int length = Moves.size();
    vector<int> count(32, 0);
    for (int i = 0; i < 4; i++) {
        if (Moves[length - 1 - i] == Moves[length - 1 - i - 4] &&
            Moves[length - 1 - i] == Moves[length - 1 - i - 8]) {
            continue;
        } else {
            return -1;
        }
    }
    //�����ظ�����
    int h_pos = 0, t_pos = 0;
    for (int i = 0; i < 32; i++) {
        if (ucsqPieces[i] == Moves[length - 1].dst) h_pos = i;
        break;
    }
    for (int i = 0; i < 32; i++) {
        if (ucsqPieces[i] == Moves[length - 2].dst) t_pos = i;
        break;
    }
    if (h_pos != 0 && h_pos != 16 && t_pos != 0 && t_pos != 16) {
        vector<vector<int>> possible = Generate();  //��ǰ״̬
        if (readyForKill(sdPlayer, ucsqPieces, possible))
            count[sdPlayer * 16]++;
        if (readyForCatch(Moves[length - 1].dst, Moves[length - 2].dst,
                          ucsqPieces, possible))
            count[t_pos]++;

        ucsqPieces[h_pos] = Moves[length - 1].src;  //���һ������
        sdPlayer = !sdPlayer;
        vector<vector<int>> possible = Generate();
        if (readyForKill(sdPlayer, ucsqPieces, possible))
            count[sdPlayer * 16]++;
        if (readyForCatch(Moves[length - 2].dst, Moves[length - 1].dst,
                          ucsqPieces, possible))
            count[h_pos]++;

        ucsqPieces[t_pos] = Moves[length - 2].src;  //�����ڶ�������
        sdPlayer = !sdPlayer;
        vector<vector<int>> possible = Generate();
        if (readyForKill(sdPlayer, ucsqPieces, possible))
            count[sdPlayer * 16]++;
        if (readyForCatch(Moves[length - 3].dst, Moves[length - 4].dst,
                          ucsqPieces, possible))
            count[h_pos]++;

        ucsqPieces[h_pos] = Moves[length - 3].src;  //��������������
        sdPlayer = !sdPlayer;
        vector<vector<int>> possible = Generate();
        if (readyForKill(sdPlayer, ucsqPieces, possible))
            count[sdPlayer * 16]++;
        if (readyForCatch(Moves[length - 4].dst, Moves[length - 3].dst,
                          ucsqPieces, possible))
            count[t_pos]++;
        //�ظ��ṹ������
        sdPlayer = !sdPlayer;
        ucsqPieces[h_pos] = Moves[length - 1].dst;
        ucsqPieces[t_pos] = Moves[length - 2].dst;

        int black = 0, red = 0;
        if (h_pos > 16) {
            red = count[t_pos] + count[0];
            black = count[h_pos] + count[16];
        } else {
            red = count[h_pos] + count[0];
            black = count[t_pos] + count[16];
        }
        if (red == black)
            return 0;
        else if (red > black)
            return 2;
        else
            return 1;
    }

    if (h_pos == 16 || t_pos == 16) return 1;
    if (h_pos == 0 || t_pos == 0) return 2;
}
