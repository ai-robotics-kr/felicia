/* global self */
/* eslint no-restricted-globals: ["off"] */
/* eslint no-bitwise: ["off"] */
import { RGBA } from 'util/color';

self.onmessage = event => {
  const { imageData, frame, scale } = event.data;
  const { angleStart, angleDelta, rangeMin, rangeMax, ranges } = frame;

  const { data, width, height } = imageData;
  const radius = Math.min(width, height) / 2;
  const size = (width * height) << 2;
  for (let i = 0; i < size; i += 1) {
    data[i] = 0;
  }

  for (let i = 0; i < ranges.length; i += 1) {
    const range = ranges[i];
    if (range >= rangeMin && range <= rangeMax) {
      const radian = angleStart + angleDelta * i;
      const r = ((radius * range) / rangeMax) * scale;
      const x = Math.floor(r * Math.sin(radian) + width / 2);
      const y = Math.floor(height / 2 - r * Math.cos(radian));

      const dataIdx = (width * y + x) << 2;
      data[dataIdx + RGBA.rIdx] = 255;
      data[dataIdx + RGBA.gIdx] = 0;
      data[dataIdx + RGBA.bIdx] = 0;
      data[dataIdx + RGBA.aIdx] = 255;
    }
  }

  self.postMessage(imageData);
};