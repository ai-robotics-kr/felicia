// Copyright (c) 2019 The Felicia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/* global self */
/* eslint no-restricted-globals: ["off"] */
import { getDataView } from '@felicia-viz/proto/messages/data-message';
import LidarFrameMessage from '@felicia-viz/proto/messages/lidar-frame-message';
import { RGBA } from '../util/color';

export interface InputEvent {
  data: {
    imageData: ImageData;
    frame: LidarFrameMessage;
    scale: number;
  };
}

export interface OutputEvent {
  data: ImageData;
}

const worker: Worker = self as any;

worker.onmessage = (event: InputEvent): void => {
  const { imageData, frame, scale } = event.data;
  const { angleStart, angleDelta, rangeMin, rangeMax, ranges } = frame;
  const rangesData = getDataView(ranges);

  const { data, width, height } = imageData;
  const radius = Math.min(width, height) / 2;
  const size = (width * height) << 2;
  for (let i = 0; i < size; i += 1) {
    data[i] = 0;
  }

  const rangesSize = rangesData.byteLength / 4;
  for (let i = 0; i < rangesSize; i += 1) {
    const range = rangesData.getFloat32(i * 4, true);
    if (range >= rangeMin && range <= rangeMax) {
      const radian = angleStart + angleDelta * i;
      const r = ((radius * range) / rangeMax) * scale;
      const x = Math.round(r * Math.sin(radian) + width / 2);
      const y = Math.round(height / 2 - r * Math.cos(radian));

      const dataIdx = (width * y + x) << 2;
      data[dataIdx + RGBA.rIdx] = 255;
      data[dataIdx + RGBA.gIdx] = 0;
      data[dataIdx + RGBA.bIdx] = 0;
      data[dataIdx + RGBA.aIdx] = 255;
    }
  }

  worker.postMessage(imageData);
};
