// Copyright (c) 2019 The Felicia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/* global self */
/* eslint no-restricted-globals: ["off"] */
import FeliciaProtoRoot from '@felicia-viz/proto/felicia-proto-root';
import { CAMERA_FRAME_MESSAGE } from '@felicia-viz/proto/messages/camera-frame-message';
import { TOPIC_INFO } from '@felicia-viz/proto/messages/master-data';
import { PixelFormat } from '@felicia-viz/proto/messages/ui';
// @ts-ignore
import Module from 'wasm/felicia_wasm.js'; // eslint-disable-line

const worker: Worker = self as any;

export interface Message {
  type: string;
  data: any;
  id: number;
}

export interface InputEvent {
  data: {
    data: string | ArrayBuffer;
    type: string;
    destinations: number[];
  };
}

export interface OutputEvent {
  data: {
    data: any;
    type: string;
    destinations: number[];
  };
}

worker.onmessage = (event: InputEvent): void => {
  let message = null;
  const { data, type, destinations } = event.data;
  if (type === TOPIC_INFO) {
    let parsed = null;
    try {
      parsed = JSON.parse(data as string);
    } catch (e) {
      console.error(e);
      return;
    }

    message = {
      data: parsed.data,
      type,
    };
  } else {
    const protoType = FeliciaProtoRoot.lookupType(type);
    if (!protoType) {
      console.error(`Don't know how to handle the message type ${type}`);
      return;
    }
    const decoded = protoType.decode(new Uint8Array(data as ArrayBuffer));
    const obj = protoType.toObject(decoded, { defaults: true });

    if (type === CAMERA_FRAME_MESSAGE) {
      const { cameraFormat } = obj;
      const { size, pixelFormat } = cameraFormat;
      const { width, height } = size;

      if (
        pixelFormat !== PixelFormat.values.PIXEL_FORMAT_BGRA &&
        pixelFormat !== PixelFormat.values.PIXEL_FORMAT_BGR &&
        pixelFormat !== PixelFormat.values.PIXEL_FORMAT_BGRX &&
        pixelFormat !== PixelFormat.values.PIXEL_FORMAT_Y8 &&
        pixelFormat !== PixelFormat.values.PIXEL_FORMAT_Y16 &&
        pixelFormat !== PixelFormat.values.PIXEL_FORMAT_RGBA &&
        pixelFormat !== PixelFormat.values.PIXEL_FORMAT_RGBX &&
        pixelFormat !== PixelFormat.values.PIXEL_FORMAT_RGB &&
        pixelFormat !== PixelFormat.values.PIXEL_FORMAT_ARGB &&
        pixelFormat !== PixelFormat.values.PIXEL_FORMAT_Z16
      ) {
        const imgSize = obj.data.byteLength;
        const bgraSize = width * height * 4;

        const imgBuffer = Module.createBuffer(imgSize);
        const bgraBuffer = Module.createBuffer(bgraSize);

        Module.HEAPU8.set(obj.data, imgBuffer);
        if (
          Module.convertToBGRA(
            imgBuffer,
            imgSize,
            bgraBuffer,
            width,
            height,
            PixelFormat.valuesById[pixelFormat]
          )
        ) {
          obj.data = new Uint8Array(Module.HEAPU8.buffer, bgraBuffer, bgraSize);
          obj.converted = true;
        }

        Module.releaseBuffer(imgBuffer);
        Module.releaseBuffer(bgraBuffer);
      }
    }

    message = {
      data: obj,
      type,
      destinations,
    };
  }

  if (message) {
    worker.postMessage(message);
  }
};
