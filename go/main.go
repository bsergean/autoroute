// Copyright 2020 Andy Bursavich. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

package main

import (
	"context"
	"flag"
	"io"
	"io/ioutil"
	"net/url"
	"strconv"
	"strings"
	"sync/atomic"
	"time"

	"go.uber.org/zap"
	"go.uber.org/zap/zapcore"
	"nhooyr.io/websocket"
)

func main() {
	timeout := flag.Duration("timeout", 5*time.Minute, "Maximum time allowed for completion of test")
	flag.Parse()

	logger, err := zap.NewDevelopment(zap.AddStacktrace(zapcore.FatalLevel + 1))
	if err != nil {
		panic(err)
	}

	target := flag.Arg(0)
	u, err := url.Parse(target)
	if err != nil {
		logger.Fatal("Invalid target URL", zap.String("url", target), zap.Error(err))
	}
	msgs, err := strconv.Atoi(strings.TrimPrefix(u.Path, "/"))
	if err != nil {
		logger.Fatal("Invalid target message count", zap.String("url", target), zap.Error(err))
	}

	ctx, cancel := context.WithTimeout(context.Background(), *timeout)
	defer cancel()

	var opts websocket.DialOptions;
	opts.CompressionMode = websocket.CompressionDisabled

	if err := run(ctx, logger, target, msgs, &opts); err != nil {
		logger.Fatal("Test incomplete", zap.Error(err))
	}
}

func run(ctx context.Context, logger *zap.Logger, url string, msgs int, opts *websocket.DialOptions) error {
	ctx, cancel := context.WithCancel(ctx)
	defer cancel()

	conn, _, err := websocket.Dial(ctx, url, opts)
	if err != nil {
		logger.Fatal("Websocket dial failed", zap.String("url", url), zap.Error(err))
	}
	defer conn.Close(websocket.StatusNormalClosure, "")

	var recv int64
	done := emitStats(ctx, logger, &recv)

	for i := 0; i < msgs; i++ {
		var r io.Reader
		if _, r, err = conn.Reader(ctx); err != nil {
			logger.Debug("Getting payload failed", zap.Error(err))
			break
		}
		if _, err = io.Copy(ioutil.Discard, r); err != nil {
			logger.Debug("Reading payload failed", zap.Error(err))
			break
		}
		atomic.AddInt64(&recv, 1)
	}
	if err != nil && ctx.Err() != nil {
		err = ctx.Err()
	}

	cancel()
	<-done

	return err
}

func emitStats(ctx context.Context, logger *zap.Logger, recv *int64) <-chan struct{} {
	start := time.Now()
	ticker := time.NewTicker(time.Second)
	done := make(chan struct{})
	go func() {
		defer close(done)
		defer ticker.Stop()

		defer func() {
			n := atomic.LoadInt64(recv)
			d := time.Since(start)
			logger.Info(
				"Overall stats",
				zap.Duration("duration", d),
				zap.Int64("messages", n),
				zap.Float64("messages/second", float64(n)/d.Seconds()),
			)
		}()

		var prev int64
		for {
			select {
			case <-ticker.C:
			case <-ctx.Done():
				return
			}
			now := atomic.LoadInt64(recv)
			logger.Info("Current stats", zap.Int64("messages/second", now-prev))
			prev = now
		}
	}()
	return done
}
