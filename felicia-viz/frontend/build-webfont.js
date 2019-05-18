const webfontsGenerator = require('webfonts-generator');
const fs = require('fs');
const path = require('path');

const dir = 'src/icons';
fs.readdir(dir, (err, filelist) => {
  if (err) throw err;

  const svgs = [];
  for (const file of filelist) {
    if (file.endsWith('.svg')) {
      svgs.push(path.resolve(dir, file));
    }
  }

  webfontsGenerator(
    {
      files: svgs,
      dest: 'src/fonts/',
      fontName: 'felicia-icons',
      templateOptions: {
        baseSelector: '.felicia-icons',
        classPrefix: 'felicia-icons-',
      },
    },
    error => {
      if (error) {
        throw error;
      }

      console.log('Done!');
    }
  );
});
