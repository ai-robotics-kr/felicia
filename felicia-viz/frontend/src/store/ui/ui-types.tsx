// Copyright (c) 2019 The Felicia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import {
  Pose3WithTimestampMessage,
  PoseWithTimestampMessage,
} from '@felicia-viz/proto/messages/geometry';
import {
  PointcloudMessage,
  POINTCLOUD_MESSAGE,
  OccupancyGridMapMessage,
  OCCUPANCY_GRID_MAP_MESSAGE,
} from '@felicia-viz/proto/messages/map-message';
import CameraFrameControlPanel from '@felicia-viz/ui/components/camera-frame-control-panel';
import CameraFrameView from '@felicia-viz/ui/components/camera-frame-view';
import DepthCameraFrameControlPanel from '@felicia-viz/ui/components/depth-camera-frame-control-panel';
import DepthCameraFrameView from '@felicia-viz/ui/components/depth-camera-frame-view';
import ImuFrameControlPanel from '@felicia-viz/ui/components/imu-frame-control-panel';
import ImuFrameView from '@felicia-viz/ui/components/imu-frame-view';
import LidarFrameControlPanel from '@felicia-viz/ui/components/lidar-frame-control-panel';
import LidarFrameView from '@felicia-viz/ui/components/lidar-frame-view';
import OccupancyGridMapControlPanel from '@felicia-viz/ui/components/occupancy-grid-map-control-panel';
import PointcloudControlPanel from '@felicia-viz/ui/components/pointcloud-control-panel';
import PoseWithTimestampControlPanel from '@felicia-viz/ui/components/pose-with-timestamp-control-panel';
import Pose3WithTimestampControlPanel from '@felicia-viz/ui/components/pose3-with-timestamp-control-panel';
import { FeliciaVizStore } from '@felicia-viz/ui/store';
import { UIType } from '@felicia-viz/ui/store/ui-state';
import CameraFrameViewState from '@felicia-viz/ui/store/ui/camera-frame-view-state';
import DepthCameraFrameViewState from '@felicia-viz/ui/store/ui/depth-camera-frame-view-state';
import ImuFrameViewState from '@felicia-viz/ui/store/ui/imu-frame-view-state';
import LidarFrameViewState from '@felicia-viz/ui/store/ui/lidar-frame-view-state';
import CameraControlPanel from 'components/camera-control-panel';
import React from 'react';
import MainSceneState from 'store/ui/main-scene-state';

const UI_TYPES: { [key: string]: UIType } = {};

export const MainSceneType: UIType = {
  name: 'MainScene',
  state: MainSceneState,
};

UI_TYPES[MainSceneType.name] = MainSceneType;

export const CameraFrameViewType: UIType = {
  name: 'CameraFrameView',
  className: 'camera-frame-view',
  state: CameraFrameViewState,
  renderView: (id: number): JSX.Element => {
    return <CameraFrameView key={id} id={id} />;
  },
  renderControlPanel: (store: FeliciaVizStore): JSX.Element => {
    const viewState = store.uiState.getActiveViewState() as CameraFrameViewState;
    const { frame } = viewState;
    return <CameraFrameControlPanel frame={frame} />;
  },
};

UI_TYPES[CameraFrameViewType.name] = CameraFrameViewType;

export const DepthCameraFrameViewType: UIType = {
  name: 'DepthCameraFrameView',
  className: 'depth-camera-frame-view',
  state: DepthCameraFrameViewState,
  renderView: (id: number): JSX.Element => {
    return <DepthCameraFrameView key={id} id={id} />;
  },
  renderControlPanel: (store: FeliciaVizStore): JSX.Element => {
    const viewState = store.uiState.getActiveViewState() as DepthCameraFrameViewState;
    const { frame, filter } = viewState;
    return <DepthCameraFrameControlPanel frame={frame} filter={filter} />;
  },
};

UI_TYPES[DepthCameraFrameViewType.name] = DepthCameraFrameViewType;

export const ImuFrameViewType: UIType = {
  name: 'ImuFrameView',
  className: 'imu-frame-view',
  state: ImuFrameViewState,
  renderView: (id: number): JSX.Element => {
    return <ImuFrameView key={id} id={id} />;
  },
  renderControlPanel: (store: FeliciaVizStore): JSX.Element => {
    const viewState = store.uiState.getActiveViewState() as ImuFrameViewState;
    const { frame, angularVelocities, linearAccelerations } = viewState;
    return (
      <ImuFrameControlPanel
        frame={frame}
        angularVelocities={angularVelocities}
        linearAccelerations={linearAccelerations}
      />
    );
  },
};

UI_TYPES[ImuFrameViewType.name] = ImuFrameViewType;

export const LidarFrameViewType: UIType = {
  name: 'LidarFrameView',
  className: 'lidar-frame-view',
  state: LidarFrameViewState,
  renderView: (id: number): JSX.Element => {
    return <LidarFrameView key={id} id={id} />;
  },
  renderControlPanel: (store: FeliciaVizStore): JSX.Element => {
    const viewState = store.uiState.getActiveViewState() as LidarFrameViewState;
    const { frame } = viewState;
    return <LidarFrameControlPanel frame={frame} />;
  },
};

UI_TYPES[LidarFrameViewType.name] = LidarFrameViewType;

export const CameraControlPanelType: UIType = {
  name: 'CameraControlPanel',
  renderControlPanel: (store: FeliciaVizStore): JSX.Element => {
    const viewState = store.uiState.getActiveViewState() as MainSceneState;
    const { followPose, position } = viewState.camera;
    return <CameraControlPanel followPose={followPose} position={position} />;
  },
};

UI_TYPES[CameraControlPanelType.name] = CameraControlPanelType;

export const OccupancyGridMapControlPanelType: UIType = {
  name: 'OccupancyGridMapControlPanel',
  renderControlPanel: (store: FeliciaVizStore): JSX.Element => {
    const viewState = store.uiState.getActiveViewState() as MainSceneState;
    const enabled = viewState.hasTopic(OCCUPANCY_GRID_MAP_MESSAGE);
    const map = viewState.map instanceof OccupancyGridMapMessage ? viewState.map : null;
    return <OccupancyGridMapControlPanel enabled={enabled} map={map} />;
  },
};

UI_TYPES[OccupancyGridMapControlPanelType.name] = OccupancyGridMapControlPanelType;

export const PointcloudControlPanelType: UIType = {
  name: 'PointcloudControlPanel',
  renderControlPanel: (store: FeliciaVizStore): JSX.Element => {
    const viewState = store.uiState.getActiveViewState() as MainSceneState;
    const enabled = viewState.hasTopic(POINTCLOUD_MESSAGE);
    const map = viewState.map instanceof PointcloudMessage ? viewState.map : null;
    return <PointcloudControlPanel enabled={enabled} map={map} />;
  },
};

UI_TYPES[PointcloudControlPanelType.name] = PointcloudControlPanelType;

export const PoseWithTimestampControlPanelType: UIType = {
  name: 'PoseWithTimestampControlPanel',
  renderControlPanel: (store: FeliciaVizStore): JSX.Element => {
    const viewState = store.uiState.getActiveViewState() as MainSceneState;
    const pose = viewState.pose instanceof PoseWithTimestampMessage ? viewState.pose : null;
    return <PoseWithTimestampControlPanel pose={pose} />;
  },
};

UI_TYPES[PoseWithTimestampControlPanelType.name] = PoseWithTimestampControlPanelType;

export const Pose3WithTimestampControlPanelType: UIType = {
  name: 'Pose3WithTimestampControlPanel',
  renderControlPanel: (store: FeliciaVizStore): JSX.Element => {
    const viewState = store.uiState.getActiveViewState() as MainSceneState;
    const pose = viewState.pose instanceof Pose3WithTimestampMessage ? viewState.pose : null;
    return <Pose3WithTimestampControlPanel pose={pose} />;
  },
};

UI_TYPES[Pose3WithTimestampControlPanelType.name] = Pose3WithTimestampControlPanelType;

export default UI_TYPES;
