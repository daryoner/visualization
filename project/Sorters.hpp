

enum class QsortStatus
{
	NewSortBorders,
	SetPivot,
	FindPair,
	SwapFound,
	SetBorders,
	ReleaseBorders,
	Finish


};

class SortBorders
{
public:
	int L, R;

	SortBorders();
	SortBorders(int, int);
};

class QuickSorter
{
public:
	QuickSorter();
	~QuickSorter();

	void NextStep(std::vector<Square*>& v);
	std::string name;

private:
	QsortStatus qSortStatus = QsortStatus::NewSortBorders;
	int i, j, mid;
	std::vector<SortBorders> sb;
	Square* pivot;

	void PrintStatus();
};

enum class BsortStatus
{
	Start,
	Compare,
	Swap,
	Finish


};

class BubbleSorter
{
public:
	BubbleSorter() { name = "BubbleSorter"; };
	~BubbleSorter() {};

	void NextStep(std::vector<Square*>& v);

	std::string name;

private:
	BsortStatus bSortStatus = BsortStatus::Start;
	int i, j;
	int externalSize, internalSize;


	void PrintStatus();
};