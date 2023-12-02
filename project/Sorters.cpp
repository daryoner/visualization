#include <Sorters.hpp>

SortBorders::SortBorders()
{

}

SortBorders::SortBorders(int left, int right)
{
	L = left;
	R = right;
}

void QuickSorter::PrintStatus()
{
	std::cout << "status: ";
	switch (qSortStatus)
	{
	case QsortStatus::NewSortBorders:// 0
		std::cout << "NewSortBorders | ";
		break;
	case QsortStatus::SetPivot: // 1
		std::cout << "SetPivot | ";
		break;
	case QsortStatus::FindPair: // 2
		std::cout << "FindPair | ";
		break;
	case QsortStatus::SwapFound: // 3
		std::cout << "SwapFound | ";
		break;
	case QsortStatus::SetBorders: // 4
		std::cout << "SetBorders | ";
		break;
	case QsortStatus::ReleaseBorders:
		std::cout << "ReleaseBorders | ";
		break;
	case QsortStatus::Finish:
		std::cout << "Finish | ";
		break;
	}
}

void QuickSorter::NextStep(std::vector<Square*>& v)
{
	Square* temp = nullptr;
	//PrintStatus();
	//std::cout << "subVectors to sort:" << sb.size() << std::endl;
	switch (qSortStatus)
	{
	case QsortStatus::NewSortBorders:// 0
		for (int lb = sb[0].L; lb <= sb[0].R; lb++)
		{
			v[lb]->setTarget(Vector2(v[lb]->GetPosition().x, 1.0f));
		}
		qSortStatus = QsortStatus::SetPivot;
		break;
	case QsortStatus::SetPivot: // 1
		i = sb[0].L;
		j = sb[0].R;
		mid = sb[0].L + (sb[0].R - sb[0].L) / 2;
		pivot = v[mid];
		//std::cout << "\tpivot index:" << mid << std::endl;
		pivot->setTarget(Vector2(pivot->GetPosition().x, -1.f));
		qSortStatus = QsortStatus::FindPair;
		break;
	case QsortStatus::FindPair: // 2



		if ((i < sb[0].R) || (j > sb[0].L))
		{
			while (v[i]->value < pivot->value)
			{
				i++;
			}
			while (v[j]->value > pivot->value)
			{
				j--;
			}
			//std::cout << "\tFirstToSwap:" << i << "\tSecondToSwap:" << j << std::endl;;

		}


		//std::cout << i << ' ' << j << '\n';
		if (i <= j)
		{
			qSortStatus = QsortStatus::SwapFound;
			v[i]->setTarget(Vector2(v[i]->GetPosition().x, -1.f));
			v[j]->setTarget(Vector2(v[j]->GetPosition().x, -1.f));
			if (i == j)
				this->NextStep(v);
		}

		else
		{
			qSortStatus = QsortStatus::SetBorders;
			this->NextStep(v);
		}

		break;
	case QsortStatus::SwapFound: // 3
		temp = v[i];
		v[i] = v[j];
		v[j] = temp;
		if (v[i] == pivot)
			v[i]->setTarget(Vector2(v[i]->GetPosition().x, -1.f));
		else
			v[i]->setTarget(Vector2(v[i]->GetPosition().x, 1.f));
		if (v[j] == pivot)
			v[j]->setTarget(Vector2(v[j]->GetPosition().x, 1.f));
		else
			v[j]->setTarget(Vector2(v[j]->GetPosition().x, 1.f));
		i++;
		j--;

		if (i >= j)
			qSortStatus = QsortStatus::SetBorders;
		else
			qSortStatus = QsortStatus::FindPair;
		break;
	case QsortStatus::SetBorders: // 4

		for (int lb = sb[0].L; lb <= sb[0].R; lb++)
		{
			v[lb]->setTarget(Vector2(v[lb]->GetPosition().x, 3.0f));
		}

		pivot->setTarget(Vector2(pivot->GetPosition().x, 3.0f));
		//std::cout << '\t' << i << " " << j << " " << sb[0].R << " " << sb[0].L << std::endl;
		if (i < sb[0].R)
			sb.push_back(SortBorders(i, sb[0].R));
		if (j > sb[0].L)
			sb.push_back(SortBorders(sb[0].L, j));

		if (sb.size() > 1)
		{
			qSortStatus = QsortStatus::SetPivot;
		}
		else
			qSortStatus = QsortStatus::Finish;
		sb.erase(sb.begin());
		if (sb.size() > 0)
			qSortStatus = QsortStatus::ReleaseBorders;



		break;
	case QsortStatus::ReleaseBorders:
		//std::cout << "\tnextBorders:\t" << sb[0].L << '\t' << sb[0].R << std::endl;
		for (int lb = sb[0].L; lb <= sb[0].R; lb++)
		{
			v[lb]->setTarget(Vector2(v[lb]->GetPosition().x, 1.0f));
		}
		qSortStatus = QsortStatus::SetPivot;


		break;

	case QsortStatus::Finish:
		std::cout << "QUICK SORTED!!!" << std::endl;
		break;
	}
}

QuickSorter::QuickSorter()
{
	sb.push_back(SortBorders(0, params::circleAmount - 1));
	//sb[0].L = 0;
	//sb[0].R = params::circleAmount - 1;
	i = sb[0].L;
	j = sb[0].R;
	mid = i + (j - i) / 2;
	pivot = nullptr;
	name = "QuickSorter";
}

QuickSorter::~QuickSorter()
{
}

void BubbleSorter::NextStep(std::vector<Square*>& v)
{
	Square* temp = nullptr;
	//std::cout << (int)bSortStatus << '\n';
	switch (bSortStatus)
	{
	case BsortStatus::Start:
		externalSize = v.size();
		internalSize = v.size() - 1;
		i = 0;
		j = 0;
		bSortStatus = BsortStatus::Compare;
		this->NextStep(v);
		break;
	case BsortStatus::Compare:
		//move down adjacent
		v[j]->setTarget(Vector2(v[j]->GetPosition().x, 1.0f));
		v[j + 1]->setTarget(Vector2(v[j + 1]->GetPosition().x, 1.0f));
		bSortStatus = BsortStatus::Swap;
		break;
	case BsortStatus::Swap:
		//swap and increment
		if (v[j]->value > v[j + 1]->value)
		{
			temp = v[j];
			v[j] = v[j + 1];
			v[j + 1] = temp;
		}
		v[j]->setTarget(Vector2(v[j + 1]->GetPosition().x, 3.0f));
		v[j]->setTarget(Vector2(v[j + 1]->GetPosition().x, 3.0f));
		j++;
		if (j == internalSize)
		{
			j = 0;
			i++;
		}
		if (i == externalSize)
		{
			bSortStatus = BsortStatus::Finish;
			this->NextStep(v);
		}
		else
			bSortStatus = BsortStatus::Compare;
		for (int a = 0; a < v.size(); a++)
		{
			v[a]->setTarget(Vector2(v[a]->GetPosition().x, 3.0f));
		}

		break;
	case BsortStatus::Finish:
		for (int a = 0; a < v.size(); a++)
		{
			v[a]->setTarget(Vector2(v[a]->GetPosition().x, 3.0f));
		}
		std::cout << "BUBBLE SORTED!!!\n";
		break;
	}

}