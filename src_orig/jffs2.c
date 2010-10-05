#include <header.h>

int
jffs2_menu (void)
{
  int ret, flag = 0;

  ret = jffs_mount_test ();

  lk_dispclr ();

  if (ret == SUCCESS)
    lk_disptext (2, 4, "JFFS2 MOUNTED", 1);
  else
    {
      lk_disptext (2, 3, "JFFS2 NOT MOUNTED", 1);
      flag = 1;
    }
  lk_disptext (5, 3, "Press Any Key", 0);
  lk_getkey ();

  lk_dispclr ();
  lk_disptext (1, 4, "ERASE JFFS2 ?", 1);
  lk_disptext (3, 3, "Removes All Data", 0);
  lk_disptext (5, 0, "ENTER->YES      X->NO", 0);
  ret = lk_getkey ();

  if (ret == CANCEL)
    return ERROR;

  if (ret == ENTER)
    {
      if (flag == 0)
	{
	  system ("umount /mnt/jffs2");
	  sleep (2);
	}

      else if (flag == 1)
	mkdir ("/mnt/jffs2", 777);


      lk_dispclr ();
      lk_disptext (2, 2, "Erasing JFFS2 ...", 1);
      system ("eraseall -j /dev/mtd5");
      sleep (2);

      system ("mount -t jffs2 /dev/mtdblock5  /mnt/jffs2/");	// Remounting jffs2

      ret = jffs_mount_test ();

      if (ret == ERROR)
	{
	  rmdir ("/mnt/jffs2");
	  lk_dispclr ();
	  lk_disptext (2, 0, "JFFS2 NOT MOUNTED", 1);
	  lk_disptext (5, 3, "Press Any Key", 0);
	  lk_getkey ();
	  return ERROR;
	}

      else
	{
	  lk_dispclr ();
	  lk_disptext (2, 4, "JFFS2 Erased", 0);
	  lk_disptext (3, 4, "Press Any Key", 0);
	  lk_getkey ();
	}

      return SUCCESS;

    }

}

int
jffs_mount_test (void)
{
  FILE *fp;
  char buff[80] = "", flag = 0;

  fp = fopen ("/etc/mtab", "r");

  while ((fgets (buff, 80, fp) != NULL))
    if (strstr (buff, "/mnt/jffs2") != NULL)
      flag = 1;
  fclose (fp);

  if (flag == 1)
    return SUCCESS;
  else
    return ERROR;
}
