#include "Evaluate.h"
inline void calculateEval(int position, int type, int& evalnum) {
	evalnum += AttackValue[type];
	evalnum += PositionValue[type][position];
}
inline int mirrorFlip(int position) { //对于黑方，请镜像翻转
	return 91 - position;
}
Eval::Eval(PositionStruct& pos) {
	EvalRed = EvalBlack = 0;
	for (int i = 0; i < 16; ++i)
		if (pos.ucsqPieces[i])
			calculateEval(pos.ucsqPieces[i], cnPieceTypes[i], EvalRed);
	for (int i = 16; i < 32; ++i)
		if (pos.ucsqPieces[i])
			calculateEval(mirrorFlip(pos.ucsqPieces[i]), cnPieceTypes[i],
				EvalBlack);
}
