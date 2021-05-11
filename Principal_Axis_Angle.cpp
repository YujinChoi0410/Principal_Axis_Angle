void Compute_Principal_Axis_Angle(BYTE **img, int width, int height, double *theta) {

	double xm00, xm01, xm10, xmu20, xmu02;
	double xmu11, xmu12, xmu21, xb, yb;
	const double Rad_to_deg = 57.2957951; //radian to degree 변환시 곱해줄 상수

	//필요한 moment 구하기
	xm00 = calmpq(0, 0, img, width, height);
	xm01 = calmpq(0, 1, img, width, height);
	xm10 = calmpq(1, 0, img, width, height);

	//물체의 중심점 좌표
	xb = xm10 / xm00;
	yb = xm01 / xm00;

	//필요한 central moment 구하기
	xmu20 = calmupq(2, 0, img, width, height, xb, yb);
	xmu02 = calmupq(0, 2, img, width, height, xb, yb);
	xmu11 = calmupq(1, 1, img, width, height, xb, yb);

	//atan2 함수를 이용해 arctan(tan역함수) 구하기
	*theta = atan2(2 * xmu11, (xmu20 - xmu02)) / 2 * Rad_to_deg;
}

void CHomework::OnClickedAxis()
{
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CChildFrame *pChildFrame = (CChildFrame *)pMainFrame->MDIGetActive(NULL);
	CImageProcessingView *pView = (CImageProcessingView *)pChildFrame->GetActiveView();
	CDC *dc = pView->GetDC();

	int i, x0, y0, w, h, x, y, xp, yp, cx, cy;	//각종 변수 설정
	char fname[100];	//입력 bmp파일 이름을 저장할 공간
	BYTE **img, **img2; //img:입력영상, img2: rotated 출력영상
	double theta;		//Princial axis angle
	char msg[100];		//물체 이미지 아래 theta값을 표시할 저장 공간
	char FIle_Name[15] = { "Ship.bmp" };	//입력 bmp파일 이름 

	x0 = 10;	//좌측 상단 모서리에서 10x10 만큼 띄우기 
	y0 = 10;


	sprintf(fname, "c:\\%s", FIle_Name);
	Display_BMP_File(fname, x0, y0);		//입력 bmp파일 display하기
	GetCurrentImageInfo(&w, &h, &x0, &y0);	//입력 영상 정보 읽기
	img = cmatrix(h, w);		// BYTE** 공간 할당하기
	GetCurrentImageGray(img);	// 회색조 영상 읽기

	Compute_Principal_Axis_Angle(img, w, h, &theta);	//theta를 계산해서 return

	sprintf(msg, "theta = %9.2f", theta);	//theta 값 display
	dc->TextOut(x0, y0 + h + 5, msg, strlen(msg));

	img2 = cmatrix(h, w);	//회전된 영상 출력공간 할당하기

	double Rotate_theta = theta * PI / 180;	//degree to radian 변환하기

	cx = (int)(w * 0.5);	//영상의 중심좌표(cx, cy)
	cy = (int)(h * 0.5);

	//theta만큼 이미지 회전하기
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			//픽셀을 원점으로 이동시켜서 회전한 후, 다시 원래 자리로 가져오기
			xp = (int)((x - cx)*cos(Rotate_theta) - (y - cy)*sin(Rotate_theta)) + cx;
			yp = (int)((x - cx)*sin(Rotate_theta) + (y - cy)*cos(Rotate_theta)) + cy;

			//할당된 공간 내에서만 이미지 출력하기
			if (xp >= 0 && xp < w && yp >= 0 && yp < h) {
				img2[y][x] = img[yp][xp];
			}
			else img2[y][x] = 0;
		}
	}

	DisplayCimage2D(img2, w, h, x0 + w + 10, y0, true); // 회색조 영상 출력
	free_cmatrix(img, h, w);	//BYTE** 할당해제
	free_cmatrix(img2, h, w);	//BYTE** 할당해제

	pView->ReleaseDC(dc);
}