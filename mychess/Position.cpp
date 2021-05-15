#include "ChessBoard.h"

inline void PositionStruct::AddPiece(int pos, int no) {  // �������Ϸ�һö����
    ucsqPieces[no] = pos;  //��no�ţ�0~32�����ӵ�λ�ã�9*10��
}
inline void PositionStruct::DelPiece(int no) {  // ������������һö����
    ucsqPieces[no] = 0;
}
inline int PosToNo(int pos, int ucsqPieces[]) {
    for (int i = 0; i < 32; ++i)
        if (pos == ucsqPieces[i]) return i;
    return -1;
}
void PositionStruct::ClearBoard() {  // �������
    sdPlayer = 0;
    memset(ucsqPieces, 0, 32 * sizeof(int));
}

inline int GetPiecePos(int i, int j) {
    return i * COL + j + 1;  //�����е�0���ţ���1~90
}
// FEN��ʶ��
void PositionStruct::FromFen(const char* const szFen) {
    int i, j, k;
    int pcRed[7];
    int pcBlack[7];
    const char* lpFen;
    // FEN����ʶ��������¼������裺
    // 1. ��ʼ�����������
    for (i = 0; i < 7; i++) {
        pcRed[i] = FROM_NUM[i];
        pcBlack[i] = pcRed[i] + 16;
    }

    //����"pcRed[7]"��"pcBlack[7]"�ֱ�����췽�ͺڷ�ÿ�����ּ���ռ�е���ţ�
    //��"pcRed[7]"Ϊ������������0��15���δ�����˧�������������������ڱ�������������
    //�������Ӧ����"pcRed[7] =
    //{0,1,3,5,7,9,11}"��ÿ����һ�����ӣ����������1��

    ClearBoard();
    lpFen = szFen;
    // 2. ��ȡ�����ϵ�����
    i = RANK_TOP;
    j = FILE_LEFT;
    while (*lpFen != ' ') {  //�ǿո�
        if (*lpFen == '/') {
            j = FILE_LEFT;
            ++i;
            if (i > RANK_BOTTOM) {
                break;
            }
        } else if (*lpFen >= '1' && *lpFen <= '9') {
            j += (*lpFen - '0');  //����������һ����
        } else if (*lpFen >= 'A' && *lpFen <= 'Z') {
            k = FenPiece[*lpFen];
            AddPiece(GetPiecePos(i, j), pcRed[k]++);
            ++j;
        } else if (*lpFen >= 'a' && *lpFen <= 'z') {
            k = FenPiece[*lpFen - 32];
            AddPiece(GetPiecePos(i, j), pcBlack[k]++);
            ++j;
        }
        ++lpFen;
    }
    while (*lpFen == ' ') ++lpFen;

    // 3. ȷ���ֵ��ķ���
    if (*lpFen == 'b') {
        sdPlayer = 1;
    }
    lpFen += 6;
    while (*lpFen != ' ') ++lpFen;
    while (*lpFen == ' ') ++lpFen;

    // 4. �غ���
    RoundNum = 0;
    while (*lpFen != ' ' && *lpFen != '\0') {
        RoundNum *= 10;
        RoundNum += *lpFen - '0';
        ++lpFen;
    }
    if (*lpFen == '\0') return;     //û��moves
    while (*lpFen == ' ') ++lpFen;  //˵���ǿո񣬺��滹��moves

    // 5. ����moves�����
    while (*lpFen != ' ') {
        ++lpFen;
        if (*lpFen == '\0') return;  //����moves��û��ʵ�ʵĶ���
    }
    while (*lpFen == ' ') ++lpFen;
    moveNode tep = {0, 0, 0};  //��ʼ������ʱ�ӵ�λ�ã��Ƿ����
    while (*lpFen != '\0') {
        tep.src = GetPiecePos('9' - *(lpFen + 1), *lpFen - 'a');
        tep.dst = GetPiecePos('9' - *(lpFen + 3), *(lpFen + 2) - 'a');
        AddPiece(tep.dst, PosToNo(tep.src, ucsqPieces));  //���ÿ��ǳ���
        Moves.push_back(tep);
        lpFen += 4;
        while (*lpFen == ' ') ++lpFen;
    }
}