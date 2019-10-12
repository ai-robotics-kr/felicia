/* eslint import/prefer-default-export: "off" */
import FeliciaProtoRoot from '../felicia-proto-root';
import { PointMessageProtobuf } from './geometry';
import { Image, ImageProtobuf } from './ui';

export const IMAGE_WITH_HUMANS_MESSAGE = 'felicia.ImageWithHumansMessage';

export const HumanBody = FeliciaProtoRoot.lookupEnum('felicia.HumanBody');
export const HumanBodyModel = FeliciaProtoRoot.lookupEnum('felicia.HumanBodyModel');

export enum HumanBodyModelProtobuf {
  HUMAN_BODY_MODEL_BODY_25 = 0,
  HUMAN_BODY_MODEL_COCO = 1,
  HUMAN_BODY_MODEL_MPI = 2,
}

export enum HumanBodyProtobuf {
  HUMAN_BODY_NONE = 0,
  HUMAN_BODY_NOSE = 1,
  HUMAN_BODY_NECK = 2,
  HUMAN_BODY_R_SHOULDER = 3,
  HUMAN_BODY_R_ELBOW = 4,
  HUMAN_BODY_R_WRIST = 5,
  HUMAN_BODY_L_SHOULDER = 6,
  HUMAN_BODY_L_ELBOW = 7,
  HUMAN_BODY_L_WRIST = 8,
  HUMAN_BODY_M_HIP = 9,
  HUMAN_BODY_R_HIP = 10,
  HUMAN_BODY_R_KNEE = 11,
  HUMAN_BODY_R_ANKLE = 12,
  HUMAN_BODY_L_HIP = 13,
  HUMAN_BODY_L_KNEE = 14,
  HUMAN_BODY_L_ANKLE = 15,
  HUMAN_BODY_R_EYE = 16,
  HUMAN_BODY_L_EYE = 17,
  HUMAN_BODY_R_EAR = 18,
  HUMAN_BODY_L_EAR = 19,
  HUMAN_BODY_L_BIG_TOE = 20,
  HUMAN_BODY_L_SMALL_TOE = 21,
  HUMAN_BODY_L_HEEL = 22,
  HUMAN_BODY_R_BIG_TOE = 23,
  HUMAN_BODY_R_SMALL_TOE = 24,
  HUMAN_BODY_R_HEEL = 25,
  HUMAN_BODY_HEAD = 26,
  HUMAN_BODY_CHEST = 27,

  HUMAN_BODY_L_HAND = 100,
  HUMAN_BODY_L_THUMB_1_CMC = 101,
  HUMAN_BODY_L_THUMB_2_KNUCKLES = 102,
  HUMAN_BODY_L_THUMB_3_IP = 103,
  HUMAN_BODY_L_THUMB_4_FINGER_TIP = 104,
  HUMAN_BODY_L_INDEX_1_KNUCKLES = 105,
  HUMAN_BODY_L_INDEX_2_PIP = 106,
  HUMAN_BODY_L_INDEX_3_DIP = 107,
  HUMAN_BODY_L_INDEX_4_FINGER_TIP = 108,
  HUMAN_BODY_L_MIDDLE_1_KNUCKLES = 109,
  HUMAN_BODY_L_MIDDLE_2_PIP = 110,
  HUMAN_BODY_L_MIDDLE_3_DIP = 111,
  HUMAN_BODY_L_MIDDLE_4_FINGER_TIP = 112,
  HUMAN_BODY_L_RING_1_KNUCKLES = 113,
  HUMAN_BODY_L_RING_2_PIP = 114,
  HUMAN_BODY_L_RING_3_DIP = 115,
  HUMAN_BODY_L_RING_4_FINGER_TIP = 116,
  HUMAN_BODY_L_PINKY_1_KNUCKLES = 117,
  HUMAN_BODY_L_PINKY_2_PIP = 118,
  HUMAN_BODY_L_PINKY_3_DIP = 119,
  HUMAN_BODY_L_PINKY_4_FINGER_TIP = 120,
  HUMAN_BODY_R_HAND = 121,
  HUMAN_BODY_R_THUMB_1_CMC = 122,
  HUMAN_BODY_R_THUMB_2_KNUCKLES = 123,
  HUMAN_BODY_R_THUMB_3_IP = 124,
  HUMAN_BODY_R_THUMB_4_FINGER_TIP = 125,
  HUMAN_BODY_R_INDEX_1_KNUCKLES = 126,
  HUMAN_BODY_R_INDEX_2_PIP = 127,
  HUMAN_BODY_R_INDEX_3_DIP = 128,
  HUMAN_BODY_R_INDEX_4_FINGER_TIP = 129,
  HUMAN_BODY_R_MIDDLE_1_KNUCKLES = 130,
  HUMAN_BODY_R_MIDDLE_2_PIP = 131,
  HUMAN_BODY_R_MIDDLE_3_DIP = 132,
  HUMAN_BODY_R_MIDDLE_4_FINGER_TIP = 133,
  HUMAN_BODY_R_RING_1_KNUCKLES = 134,
  HUMAN_BODY_R_RING_2_PIP = 135,
  HUMAN_BODY_R_RING_3_DIP = 136,
  HUMAN_BODY_R_RING_4_FINGER_TIP = 137,
  HUMAN_BODY_R_PINKY_1_KNUCKLES = 138,
  HUMAN_BODY_R_PINKY_2_PIP = 139,
  HUMAN_BODY_R_PINKY_3_DIP = 140,
  HUMAN_BODY_R_PINKY_4_FINGER_TIP = 141,

  HUMAN_BODY_FACE_COUNTOUR_0 = 160,
  HUMAN_BODY_FACE_COUNTOUR_1 = 161,
  HUMAN_BODY_FACE_COUNTOUR_2 = 162,
  HUMAN_BODY_FACE_COUNTOUR_3 = 163,
  HUMAN_BODY_FACE_COUNTOUR_4 = 164,
  HUMAN_BODY_FACE_COUNTOUR_5 = 165,
  HUMAN_BODY_FACE_COUNTOUR_6 = 166,
  HUMAN_BODY_FACE_COUNTOUR_7 = 167,
  HUMAN_BODY_FACE_COUNTOUR_8 = 168,
  HUMAN_BODY_FACE_COUNTOUR_9 = 169,
  HUMAN_BODY_FACE_COUNTOUR_10 = 170,
  HUMAN_BODY_FACE_COUNTOUR_11 = 171,
  HUMAN_BODY_FACE_COUNTOUR_12 = 172,
  HUMAN_BODY_FACE_COUNTOUR_13 = 173,
  HUMAN_BODY_FACE_COUNTOUR_14 = 174,
  HUMAN_BODY_FACE_COUNTOUR_15 = 175,
  HUMAN_BODY_FACE_COUNTOUR_16 = 176,
  HUMAN_BODY_R_EYE_BROW_0 = 177,
  HUMAN_BODY_R_EYE_BROW_1 = 178,
  HUMAN_BODY_R_EYE_BROW_2 = 179,
  HUMAN_BODY_R_EYE_BROW_3 = 180,
  HUMAN_BODY_R_EYE_BROW_4 = 181,
  HUMAN_BODY_L_EYE_BROW_4 = 182,
  HUMAN_BODY_L_EYE_BROW_3 = 183,
  HUMAN_BODY_L_EYE_BROW_2 = 184,
  HUMAN_BODY_L_EYE_BROW_1 = 185,
  HUMAN_BODY_L_EYE_BROW_0 = 186,
  HUMAN_BODY_NOSE_UPPER_0 = 187,
  HUMAN_BODY_NOSE_UPPER_1 = 188,
  HUMAN_BODY_NOSE_UPPER_2 = 189,
  HUMAN_BODY_NOSE_UPPER_3 = 190,
  HUMAN_BODY_NOSE_LOWER_0 = 191,
  HUMAN_BODY_NOSE_LOWER_1 = 192,
  HUMAN_BODY_NOSE_LOWER_2 = 193,
  HUMAN_BODY_NOSE_LOWER_3 = 194,
  HUMAN_BODY_NOSE_LOWER_4 = 195,
  HUMAN_BODY_R_EYE_0 = 196,
  HUMAN_BODY_R_EYE_1 = 197,
  HUMAN_BODY_R_EYE_2 = 198,
  HUMAN_BODY_R_EYE_3 = 199,
  HUMAN_BODY_R_EYE_4 = 200,
  HUMAN_BODY_R_EYE_5 = 201,
  HUMAN_BODY_L_EYE_0 = 202,
  HUMAN_BODY_L_EYE_1 = 203,
  HUMAN_BODY_L_EYE_2 = 204,
  HUMAN_BODY_L_EYE_3 = 205,
  HUMAN_BODY_L_EYE_4 = 206,
  HUMAN_BODY_L_EYE_5 = 207,
  HUMAN_BODY_O_MOUSE_0 = 208,
  HUMAN_BODY_O_MOUSE_1 = 209,
  HUMAN_BODY_O_MOUSE_2 = 210,
  HUMAN_BODY_O_MOUSE_3 = 211,
  HUMAN_BODY_O_MOUSE_4 = 212,
  HUMAN_BODY_O_MOUSE_5 = 213,
  HUMAN_BODY_O_MOUSE_6 = 214,
  HUMAN_BODY_O_MOUSE_7 = 215,
  HUMAN_BODY_O_MOUSE_8 = 216,
  HUMAN_BODY_O_MOUSE_9 = 217,
  HUMAN_BODY_O_MOUSE_10 = 218,
  HUMAN_BODY_O_MOUSE_11 = 219,
  HUMAN_BODY_I_MOUSE_0 = 220,
  HUMAN_BODY_I_MOUSE_1 = 221,
  HUMAN_BODY_I_MOUSE_2 = 222,
  HUMAN_BODY_I_MOUSE_3 = 223,
  HUMAN_BODY_I_MOUSE_4 = 224,
  HUMAN_BODY_I_MOUSE_5 = 225,
  HUMAN_BODY_I_MOUSE_6 = 226,
  HUMAN_BODY_I_MOUSE_7 = 227,
  HUMAN_BODY_R_PUPIL = 228,
  HUMAN_BODY_L_PUPIL = 229,
}

export interface HumanBodyMessageProtobuf {
  humanBody: HumanBodyProtobuf;
  position: PointMessageProtobuf;
  score: number;
}

export interface HumanMessageProtobuf {
  humanBodies: Array<HumanBodyMessageProtobuf>;
}

export interface ImageWithHumansMessageProtobuf {
  image: ImageProtobuf;
  model: HumanBodyModelProtobuf;
  humans: Array<HumanMessageProtobuf>;
}

export class ImageWithHumansMessage {
  image: Image;

  model: HumanBodyModelProtobuf;

  humans: Array<HumanMessageProtobuf>;

  constructor({ image, model, humans }: ImageWithHumansMessageProtobuf) {
    this.image = new Image(image);
    this.model = model;
    this.humans = humans;
  }
}