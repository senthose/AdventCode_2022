#pragma once

#include <string>
#include <map>
#include <functional>
#include <limits>
#include <iostream>

namespace AdventUtility {
	const int MIN_INT = std::numeric_limits<int>::min();
	const int MAX_INT = std::numeric_limits<int>::max();

	struct Point {
		int X = 0;
		int Y = 0;

		bool operator==(const Point& inOther) const;
		bool operator!=(const Point& inOther) const;

		Point operator+(const Point& inRight) const;
		Point operator-(const Point& inRight) const;
		
		Point& operator+=(const Point& inOther);

		static int GetManhattenDistance(const Point& inPointA, const Point& inPointB);
	};

	struct ComparePoint {
		bool operator()(const Point& inLeft, const Point& inRight) const;
	};

	struct Forp {
		int someData;
	};

	template<typename T>
	class DynamicMap {
	public:
		typedef std::map<Point, T, ComparePoint> MapType;
		typedef std::function<void(const Point&, T&)> MapFunction;
		typedef std::function<void(const Point&, const T&)> ConstMapFunction;

		DynamicMap();

		T& Get(const Point& inPoint);
		T& Get(int X, int Y);
		
		const T& Get(const Point& inPoint) const;
		const T& Get(int X, int Y) const;
		
		bool Exists(const Point& inPoint) const;

		void GetExtents(int& outLeft, int& outRight, int& outTop, int& outBottom) const;

		void Foreach(MapFunction inFunction);
		void Foreach(ConstMapFunction inConstFunction) const;

		void DebugPrint(ConstMapFunction inPrintFunction, const char inEmptyChar) const;
	private:
		void UpdateExtents(const Point& inPoint);

		MapType Map;
		int LeftExtent;
		int RightExtent;
		int TopExtent;
		int BottomExtent;
	};

	template<typename T>
	DynamicMap<T>::DynamicMap()
		:Map()
		,LeftExtent(MAX_INT)
		,RightExtent(MIN_INT)
		,TopExtent(MAX_INT)
		,BottomExtent(MIN_INT)
	{
	}

	template<typename T>
	T& DynamicMap<T>::Get(const Point& inPoint)
	{
		T& t = Map[inPoint];
		UpdateExtents(inPoint);
		return t;
	}

	template<typename T>
	T& DynamicMap<T>::Get(int X, int Y)
	{
		const Point point { X, Y };
		T& t = Map[point];
		UpdateExtents(point);
		return t;
	}

	template<typename T>
	const T& DynamicMap<T>::Get(const Point& inPoint) const
	{
		return Map.at(inPoint);
	}

	template<typename T>
	const T& DynamicMap<T>::Get(int X, int Y) const
	{
		return Map.at(Point{X, Y});
	}

	template<typename T>
	bool DynamicMap<T>::Exists(const Point& inPoint) const
	{
		auto it = Map.find(inPoint);

		return it != Map.end();
	}

	template<typename T>
	void DynamicMap<T>::GetExtents(int& outLeft, int& outRight, int& outTop, int& outBottom) const
	{
		outLeft = LeftExtent;
		outRight = RightExtent;
		outTop = TopExtent;
		outBottom = BottomExtent;
	}

	template<typename T>
	void DynamicMap<T>::Foreach(MapFunction inFunction)
	{
		for (auto it = Map.begin(); it != Map.end(); ++it)
		{
			inFunction(it->first, it->second);
		}
	}

	template<typename T>
	void DynamicMap<T>::Foreach(ConstMapFunction inConstFunction) const
	{
		for (auto it = Map.begin(); it != Map.end(); ++it)
		{
			inConstFunction(it->first, it->second);
		}
	}

	template<typename T>
	void DynamicMap<T>::DebugPrint(ConstMapFunction inPrintFunction, const char inEmptyChar) const
	{
		Point printPoint;
		for (printPoint.Y = TopExtent; printPoint.Y <= BottomExtent; ++printPoint.Y)
		{
			for (printPoint.X = LeftExtent; printPoint.X <= RightExtent; ++printPoint.X)
			{
				auto it = Map.find(printPoint);
				if (it != Map.end())
				{
					inPrintFunction(it->first, it->second);
				}
				else
				{
					std::cout << inEmptyChar;
				}
			}
		}
		std::cout << std::endl;
	}

	template<typename T>
	void DynamicMap<T>::UpdateExtents(const Point& inPoint)
	{
		if (inPoint.X < LeftExtent)
		{
			LeftExtent = inPoint.X;
		}

		if (inPoint.X > RightExtent)
		{
			RightExtent = inPoint.X;
		}

		if (inPoint.Y < TopExtent)
		{
			TopExtent = inPoint.Y;
		}

		if (inPoint.Y > BottomExtent)
		{
			BottomExtent = inPoint.Y;
		}
	}
}