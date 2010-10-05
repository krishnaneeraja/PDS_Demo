#include <header.h>
int
scroll_menu (MENU_T * menu, int *selItem, int acceptKbdEvents)
{
  int i = 0, sel = 0, Maxdispcnt = 0, upflg = 0, lastitem = 0, hpos = 0, cnt =
    0, hflg = 0, event;
  char *p, rowno, menuno[3];
  short scroll, loop = 1;

  sel = *selItem;
  scroll = 0;

  if (RES32 > menu->maxEntries)
    Maxdispcnt = menu->maxEntries;
  else
    Maxdispcnt = RES32;

  printf ("Maxdispcnt=%d title=%s \n", Maxdispcnt, menu->title);
  lk_dispclr ();
begin:
  sprintf (menuno, "%2d", sel + 1);
  lk_disptext (UprOffset, 19, menuno, 0);	//menu no display
  lk_disptext (UprOffset, 3, menu->title, 0);
  if (sel < Maxdispcnt)
    rowno = 0;
  else
    rowno = sel - 3;
  cnt = 0;

  for (i = rowno; i <= rowno + (Maxdispcnt - 1); i++)
    {
      p = menu->menu[i];
      lk_disptext (UprOffset + 1 + cnt, 0, p, 0);
      cnt++;
    }

  if (sel < Maxdispcnt)
    {
      lk_disphlight (sel + UprOffset + 1);
      cnt = sel + 1;
    }
  else
    {
      lk_disphlight (UprOffset + Maxdispcnt);
      cnt = Maxdispcnt;
    }

  loop = 1;
  scroll = 0;
  hpos = 1;
  rowno = 0;

  if (sel == 0)
    cnt = 1;
  lastitem = cnt;
  while (loop)
    {
      if ((scroll == 1) && (sel < (menu->maxEntries)))
	{
	  if (upflg == 1)
	    rowno = sel - (Maxdispcnt - 1);
	  else
	    rowno = sel;

	  if (rowno <= 0)
	    rowno = 0;
	  for (i = 1; i <= Maxdispcnt; i++)
	    {
	      p = menu->menu[rowno];
	      //menuno=i;
	      lk_displineclr (UprOffset + i);
	      //lk_disptext(UprOffset,18,&menuno,0);//
	      lk_disptext (UprOffset + i, 0, p, 0);	//1
	      rowno++;
	    }

	}


      if (hflg == 1)
	{

	  if (sel <= Maxdispcnt)
	    hpos = sel;

	  if (lastitem != cnt)
	    lk_disphlight (lastitem + UprOffset);
	  lk_disphlight (cnt + UprOffset);
	  lastitem = cnt;

	}

      scroll = 0;
      upflg = 0;
      hflg = 0;

      while (1)
	{
	  event = lk_getkey ();
	  if (event != -1)
	    {
	      printf ("######event$$$$$$$$=%d\n", event);
	      break;
	    }
	}

      switch (event)
	{

	case F1:
	  sel = 0;
	  sprintf (menuno, "%2d", sel + 1);
	  lk_disptext (UprOffset, 19, menuno, 0);
	  cnt = 1;
	  lk_dispclr ();
	  goto begin;
	  break;

	case F4:
	  sel = menu->maxEntries - 1;
	  sprintf (menuno, "%2d", sel);
	  lk_disptext (UprOffset, 19, menuno, 0);
	  cnt = Maxdispcnt;
	  lk_dispclr ();
	  goto begin;
	  break;

	case F3:
	  if (sel <= (menu->maxEntries - 1))
	    {

	      ++sel;
	      sprintf (menuno, "%2d", sel + 1);
	      lk_disptext (UprOffset, 19, menuno, 0);
	      if (sel > (menu->maxEntries - 1))
		{
		  sel = 0;
		  cnt = 1;
		  lk_dispclr ();
		  goto begin;
		}
	      upflg = 1;
	      hflg = 1;
	      if (cnt < Maxdispcnt)
		cnt++;
	      else
		scroll = 1;
	    }
	  break;
	case F2:
	  if (sel >= 0)
	    {
	      sprintf (menuno, "%2d", sel);
	      lk_disptext (UprOffset, 19, menuno, 0);
	      upflg = 0;
	      sel--;
	      if (sel < 0)
		{
		  sel = menu->maxEntries - 1;
		  cnt = Maxdispcnt;
		  lk_dispclr ();
		  goto begin;
		}
	      hflg = 1;
	      if (cnt > 1)
		cnt--;
	      else
		scroll = 1;
	    }
	  break;

	case ENTER:
	  loop = 0;
	  break;

	case CANCEL:
	  loop = 0;
	  break;

	case 0x0a:
	case 0x00:
	  continue;


	default:
	  if (event > 0 && event <= menu->maxEntries
	      && acceptKbdEvents == TRUE)
	    {
	      if (event != ENTER)
		sel = event - 1;
	      loop = 0;
	    }
	  else
	    {
	      break;
	    }
	}
    }

  *selItem = sel;

  return event;

}
