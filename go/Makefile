test:
	go test
	revive -set_exit_status
	golangci-lint run -E wsl,wrapcheck,nlreturn,revive,noctx,gocritic,errorlint,forcetypeassert -p bugs -D gosec,errchkjson
