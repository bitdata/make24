#pragma once

// 运算
const __int8 ADD =1;
const __int8 SUBTRACT =2;
const __int8 MULTIPLY =3;
const __int8 DIVIDE =4;
const __int8 PROPER_FRACTION =5;
const __int8 NUM_OPERATION =5;

// 花色
const __int8 CLUBS =0;
const __int8 DIAMONDS =1;
const __int8 HEARTS =2;
const __int8 SPADE =3;
const __int8 NUM_SUIT =4;

// 难度
const int DIFFICULTY_LOWEST =1;
const int DIFFICULTY_LOW =2;
const int DIFFICULTY_NORMAL =3;
const int DIFFICULTY_HIGH =4;		
const int DIFFICULTY_HIGHEST =5;	// 采用分数运算
const int DIFFICULTY_INVALID =-1;

const int NUM_OPERAND =4;			// 操作数数量
const short RESULT_VALUE =24;		// 结果

// 表达式节点
typedef struct _EXPRESSION
{
	short		nNumerator;			// 分子
	short		nDenominator;		// 分母
	__int8		iIndex;				// 在数组中的索引
	__int8		iOperator;			// 运算
	__int8		iLeft;				// 左子节点索引
	__int8		iRight;				// 右子节点索引
}EXPRESSION;

typedef struct _KEY
{
	TCHAR		szExpression[16];
}KEY;

inline EXPRESSION* Left(EXPRESSION* pExpression)
{
	return pExpression+pExpression->iLeft-pExpression->iIndex;
}
inline const EXPRESSION* Left(const EXPRESSION* pExpression)
{
	return pExpression+pExpression->iLeft-pExpression->iIndex;
}
inline EXPRESSION* Right(EXPRESSION* pExpression)
{
	return pExpression+pExpression->iRight-pExpression->iIndex;
}
inline const EXPRESSION* Right(const EXPRESSION* pExpression)
{
	return pExpression+pExpression->iRight-pExpression->iIndex;
}

// 实现算法
class CMake24
{
public:
	void Attach(const short pOperands[]);
	bool AutoOperate(bool fAllKey =true);
	int GetKeys(KEY pKeys[] ,int nMax) const;

	static int WINAPI ToString(const EXPRESSION* pExpression, TCHAR pszExpression[], int nMax);

#if defined(_DEBUG)
	int GetDifficulty() const;
	WORD GetFirstStep() const;
	static void MakeTable(short nMax, short nMin);
#endif

private:
	struct KEYDIFFICULTY
	{
		KEY		key;
#if defined(_DEBUG)
		int		nDifficulty;
		// 第一步
		__int8		iReserved;			//
		__int8		iOperator;			// 运算
		__int8		iLeft;				// 左子节点索引
		__int8		iRight;				// 右子节点索引
#endif
	};
	typedef CAtlArray<KEYDIFFICULTY>	CKeys;		// 答案列表
	
	EXPRESSION		m_pExpressions[NUM_OPERAND*2-1];
	CKeys			m_keys;					// 记录压缩后的答案列表
	
	bool RecursiveOperate(
		bool (WINAPI pfnOperate)(EXPRESSION&,const EXPRESSION&,const EXPRESSION&, __int8),
		bool fAllKey);
	bool RecursiveFractionalOperate(bool fAllKey);
	bool HasUsed(__int8 iIndex) const;
	
	static int __cdecl CompareKey(void* , const void* pKey1, const void* pKey2);

	static void WINAPI Normalize(EXPRESSION pExpressions[]);
	static void WINAPI Sort(EXPRESSION* pExpression);
	static int WINAPI CompareExpression(const EXPRESSION& a, const EXPRESSION& b);

#if defined(_DEBUG)
	static int WINAPI GetDifficulty(const EXPRESSION pExpressions[]);
#endif
};

class CStore
{
public:
	CStore();
	void Init(int nDifficultyMin, int nDifficultyMax, bool fIncludeJQK);
	bool Rand(short pOperands[]);
	void Add(const short pOperands[]);
	void Remove(const short pOperands[]);
	bool IsEmpty() const;

	static bool GetFirstStep(const short pOperands[], __int8& iOperator, __int8& iLeft, __int8& iRight);
private:
	CAtlArray<WORD>	m_questions;
};

class CGame
{
public:
	CGame();
	void Init();
	bool BeginQuestion();
	bool EndQuestion();
	bool IsActive() const { return !m_steps.IsEmpty(); }

	bool IsResolved(KEY* pKey =NULL) const;
	int GetKeys(KEY pKeys[] ,int nMax);
	const EXPRESSION* GetExpression(__int8 iPosition) const;
	__int8 GetSuit(__int8 iIndex) const {return m_pSuits[iIndex]; }

	int GetDifficulty() const { return m_nDifficulty; }
	bool IsIncludeJQK() const { return m_fIncludeJQK; }
	bool IsKnowledgeware() const { return m_fKnowledgeware; }
	bool IsSemiautomatic() const { return m_fSemiautomatic; }
	bool IsFractional() const { return (DIFFICULTY_HIGHEST==m_nDifficulty); }
	void SetDifficulty(int nDifficulty)
	{
		m_nDifficulty =nDifficulty;
	#if !defined(_FRACTIONAL_)
		if(DIFFICULTY_HIGHEST==nDifficulty)
		{
			m_nDifficulty =DIFFICULTY_HIGH;
		}
	#endif
	}
	void SetOption(int nDifficulty, bool fIncludeJQK, bool fKnowledgeware, bool fSemiautomatic);

	int GetSuccessCount() const { return m_nSuccess; }
	int GetFailCount() const { return m_nFail; }
	int GetSuccessHistories(KEY* pKeys, int nMax) const;
	
	bool CanMerge(__int8 iFrom, __int8 iTo, __int8 iOperator) const;
	bool Merge(__int8 iFrom, __int8 iTo, __int8 iOperator);
	bool CanChangeOperator(__int8 iPosition, __int8 iOperator) const;
	bool ChangeOperator(__int8 iPosition, __int8 iOperator);
	bool CanUndo() const {return m_steps.GetCount()>1; }
	bool CanStep() const { return (m_steps.GetCount()==1); }
	__int8 Step();
	bool Undo();
	void UndoAll();
	
private:
	struct STEP
	{
		__int8		pPositions[NUM_OPERAND];
		EXPRESSION	pExpressions[NUM_OPERAND*2-1];
	};
	typedef CAtlArray<STEP>	CSteps;
	typedef CAtlArray<KEY> CHistories;
	
	CMake24		m_make24;
	CStore		m_store;

	int			m_nDifficulty;
	bool		m_fIncludeJQK;
	bool		m_fKnowledgeware;
	bool		m_fSemiautomatic;
	
	__int8		m_pSuits[NUM_OPERAND];
	CSteps		m_steps;
	
	int			m_nSuccess;
	int			m_nFail;
	CHistories	m_histories;

	bool CanOperate(__int8 iLeft, __int8 iRight, __int8 iOperator) const;
};