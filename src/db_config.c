#include "header.h"

char *get_config(char *key, char *value, char *office_id) {
	char sql[500];
	resultset ptable;

	fprintf(fp, "Begin of get_config()");
	fflush(fp);

	sprintf(sql,
			"SELECT VALUE FROM CONFIGURATION WHERE KEY='%s' AND OFFICE_ID='%s'",
			key, office_id);
	fprintf(fp, "This is sql:%s",sql);
	fflush(fp);
	ptable = get_result(sql);

	if (ptable.rows > 0) {
		strcpy(value, ptable.recordset[0][0]);
	} 
	
	fprintf(fp, "End of get_config:%s", value);
	fflush(fp);

	free_result(ptable);

	return value;
}

char *get_prop(char *key, char *value, char *office_id) {
	char sql[500];
	resultset ptable;

	fprintf(fp, "Begin of get_prop()");
	fflush(fp);

	sprintf(sql,
			"SELECT VALUE FROM PROPERTIES WHERE KEY='%s' AND OFFICE_ID='%s'",
			key, office_id);

	ptable = get_result(sql);

	if (ptable.rows > 0) {
		strcpy(value, ptable.recordset[0][0]);
	}
	
	fprintf(fp, "End of get_prop:%s", value);
	fflush(fp);

	free_result(ptable);

	return value;
}
