import React from 'react';

import CameraPanel, { CameraControlPanel } from 'components/camera-panel';
import DepthCameraPanel, { DepthCameraControlPanel } from 'components/depth-camera-panel';
import ImuPanel, { ImuControlPanel } from 'components/imu-panel';
import CameraPanelState from 'store/ui/camera-panel-state';
import DepthCameraPanelState from 'store/ui/depth-camera-panel-state';
import ImuPanelState from 'store/ui/imu-panel-state';

const UI_TYPES = {};

export const CameraPanelType = {
  name: 'CameraPanel',
  className: 'camera-panel',
  state: CameraPanelState,
  renderView: id => {
    return <CameraPanel key={id} id={id} />;
  },
  renderControlView: () => {
    return <CameraControlPanel />;
  },
};

UI_TYPES[CameraPanelType.name] = CameraPanelType;

export const DepthCameraPanelType = {
  name: 'DepthCameraPanel',
  className: 'depth-camera-panel',
  state: DepthCameraPanelState,
  renderView: id => {
    return <DepthCameraPanel key={id} id={id} />;
  },
  renderControlView: () => {
    return <DepthCameraControlPanel />;
  },
};

UI_TYPES[DepthCameraPanelType.name] = DepthCameraPanelType;

export const ImuPanelType = {
  name: 'ImuPanel',
  className: 'imu-panel',
  state: ImuPanelState,
  renderView: id => {
    return <ImuPanel key={id} id={id} />;
  },
  renderControlView: () => {
    return <ImuControlPanel />;
  },
};

UI_TYPES[ImuPanelType.name] = ImuPanelType;

export default UI_TYPES;