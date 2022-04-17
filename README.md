# Desktop Streamer
Desktop Streamer is a server that streams the screen of the desktop in RTSP, HLS, etc.  

## 개발중 
- 지금까지 개발했던 기술, 관심있는 기술을 활용하여 개발중입니다.
- 시간나는데로 틈틈히 업데이트 중입니다.
- 비디오 관련 기술들을 모두(?) 적용해보는게 목표입니다.
- 전송프로토콜(RTSP, HLS(mpeg2ts, CMAF), WebRTC)
- 화질평가(PSNR, SSIM, VMAF) (화질평가 기술이 적용할만한데가 있을지 고민중)

## 앞으로 해야할일
- 윈도우 서비스로 동작하도록 작성(완료)
- UDS(Unix Domain Socket)을 통한 통신구조 작성(진행중, 서버측 초안 구현 완료, 클라이언트측 구현 예정)
- 데스크탑 캡처 구현(desktop duplication api)
- 쉐이더로 bgr to yuv 구현
- HW인코더 구현
- RTSP서버 구현(?)
- HLS 구현(ffmpeg으로)
- 간단한 웹서버 구현(HLS 전송을 위해)
