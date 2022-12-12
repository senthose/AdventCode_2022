#pragma once

#include "AdventParser.h"
#include <functional>

namespace MonkeyBusiness {
	typedef size_t BigValue;
	
	struct Item {
		BigValue Value;
		BigValue NumOverflows = 0;
	};

	typedef std::vector<BigValue> ItemVector;

	enum class OperationType {
		Add,
		Multiply,
		Invalid,
	};

	struct MonkeyOperation {
		void SetOperation(const std::string& inLeft, const std::string& inOpString, const std::string& inRight);

		BigValue operator() (BigValue inOld);
	private:
		std::function<BigValue(BigValue)> CachedFunction;
	};

	struct MonkeyThrow {
		int TargetMonkey;
		BigValue ItemValue;
	};

	typedef std::vector<MonkeyThrow> ThrowVector;

	class Monkey {
	public:
		Monkey();

		void AddItem(BigValue inWorryValue);
		void SetOperation(const std::string& inLeft, const std::string& inOpString, const std::string& inRight);
		void SetTestValue(int inTestValue);
		void SetTrueTarget(int inTarget);
		void SetFalseTarget(int inTarget);

		void TakeTurn(ThrowVector& outThrows, bool inWorried);

		BigValue GetNumItemsInspected() const { return NumItemsInspected; }
	private:
		ItemVector HeldItems;
		MonkeyOperation Operation;
		int TestValue;
		int TrueTarget;
		int FalseTarget;
		BigValue NumItemsInspected;
	};

	class MonkeyGroup {
	public:
		MonkeyGroup();

		void AddMonkey();
		Monkey& GetMonkey(int inMonkeyIndex);

		void PerformRound(bool inWorried);

		BigValue GetCurrentMonkeyBusinessValue() const;
	private:
		std::vector<Monkey> Monkeys;
	};

	class MonkeyParser : public AdventParser::ParserInterface {
	public:
		MonkeyParser();

		void OnStartParse() override;
		void ParseLine(const int inLineNumber, const std::string& inLine) override;
		void OnEndParse() override;

		void DoRounds(BigValue inNumRounds);

		BigValue GetMonkeyBusinessValue() const;

		void SetWorried(bool inWorried) { Worried = inWorried; }
	private:
		MonkeyGroup Monkeys;
		int CurrentMonkeyBeingParsed;
		bool Worried;
	};
}