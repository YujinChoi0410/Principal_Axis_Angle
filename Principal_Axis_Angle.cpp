void Compute_Principal_Axis_Angle(BYTE **img, int width, int height, double *theta) {

	double xm00, xm01, xm10, xmu20, xmu02;
	double xmu11, xmu12, xmu21, xb, yb;
	const double Rad_to_deg = 57.2957951; //radian to degree ��ȯ�� ������ ���

	//�ʿ��� moment ���ϱ�
	xm00 = calmpq(0, 0, img, width, height);
	xm01 = calmpq(0, 1, img, width, height);
	xm10 = calmpq(1, 0, img, width, height);

	//��ü�� �߽��� ��ǥ
	xb = xm10 / xm00;
	yb = xm01 / xm00;

	//�ʿ��� central moment ���ϱ�
	xmu20 = calmupq(2, 0, img, width, height, xb, yb);
	xmu02 = calmupq(0, 2, img, width, height, xb, yb);
	xmu11 = calmupq(1, 1, img, width, height, xb, yb);

	//atan2 �Լ��� �̿��� arctan(tan���Լ�) ���ϱ�
	*theta = atan2(2 * xmu11, (xmu20 - xmu02)) / 2 * Rad_to_deg;
}

void CHomework::OnClickedAxis()
{
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CChildFrame *pChildFrame = (CChildFrame *)pMainFrame->MDIGetActive(NULL);
	CImageProcessingView *pView = (CImageProcessingView *)pChildFrame->GetActiveView();
	CDC *dc = pView->GetDC();

	int i, x0, y0, w, h, x, y, xp, yp, cx, cy;	//���� ���� ����
	char fname[100];	//�Է� bmp���� �̸��� ������ ����
	BYTE **img, **img2; //img:�Է¿���, img2: rotated ��¿���
	double theta;		//Princial axis angle
	char msg[100];		//��ü �̹��� �Ʒ� theta���� ǥ���� ���� ����
	char FIle_Name[15] = { "Ship.bmp" };	//�Է� bmp���� �̸� 

	x0 = 10;	//���� ��� �𼭸����� 10x10 ��ŭ ���� 
	y0 = 10;


	sprintf(fname, "c:\\%s", FIle_Name);
	Display_BMP_File(fname, x0, y0);		//�Է� bmp���� display�ϱ�
	GetCurrentImageInfo(&w, &h, &x0, &y0);	//�Է� ���� ���� �б�
	img = cmatrix(h, w);		// BYTE** ���� �Ҵ��ϱ�
	GetCurrentImageGray(img);	// ȸ���� ���� �б�

	Compute_Principal_Axis_Angle(img, w, h, &theta);	//theta�� ����ؼ� return

	sprintf(msg, "theta = %9.2f", theta);	//theta �� display
	dc->TextOut(x0, y0 + h + 5, msg, strlen(msg));

	img2 = cmatrix(h, w);	//ȸ���� ���� ��°��� �Ҵ��ϱ�

	double Rotate_theta = theta * PI / 180;	//degree to radian ��ȯ�ϱ�

	cx = (int)(w * 0.5);	//������ �߽���ǥ(cx, cy)
	cy = (int)(h * 0.5);

	//theta��ŭ �̹��� ȸ���ϱ�
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			//�ȼ��� �������� �̵����Ѽ� ȸ���� ��, �ٽ� ���� �ڸ��� ��������
			xp = (int)((x - cx)*cos(Rotate_theta) - (y - cy)*sin(Rotate_theta)) + cx;
			yp = (int)((x - cx)*sin(Rotate_theta) + (y - cy)*cos(Rotate_theta)) + cy;

			//�Ҵ�� ���� �������� �̹��� ����ϱ�
			if (xp >= 0 && xp < w && yp >= 0 && yp < h) {
				img2[y][x] = img[yp][xp];
			}
			else img2[y][x] = 0;
		}
	}

	DisplayCimage2D(img2, w, h, x0 + w + 10, y0, true); // ȸ���� ���� ���
	free_cmatrix(img, h, w);	//BYTE** �Ҵ�����
	free_cmatrix(img2, h, w);	//BYTE** �Ҵ�����

	pView->ReleaseDC(dc);
}