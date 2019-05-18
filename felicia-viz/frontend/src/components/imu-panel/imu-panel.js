import React, { Component } from 'react';
import PropTypes from 'prop-types';
import { inject, observer } from 'mobx-react';
import { FloatPanel } from '@streetscape.gl/monochrome';

import Activatable from 'components/activatable';
import { FLOAT_PANEL_STYLE } from 'custom-styles';
import UI_TYPES from 'store/ui/ui-types';

@inject('store')
@observer
export default class ImuPanel extends Component {
  static propTypes = {
    id: PropTypes.number.isRequired,
    store: PropTypes.object.isRequired,
  };

  constructor(props) {
    super(props);

    this.floatPanelSettings = {
      movable: true,
      minimizable: false,
      resizable: true,
    };

    this.state = {
      panelState: {
        x: window.innerWidth - 500,
        y: 20,
        width: 480, // constant for a while. This should be modified in the future.
        height: 320, // constant for a while. This should be modified in the future.
      },
    };
  }

  _onUpdate = panelState => {
    this.setState({
      panelState: {
        ...panelState,
      },
    });
  };

  render() {
    const { panelState } = this.state;
    const { id } = this.props;

    return (
      <FloatPanel
        {...panelState}
        {...this.floatPanelSettings}
        onUpdate={this._onUpdate}
        style={FLOAT_PANEL_STYLE}>
        <Activatable id={id} type={UI_TYPES.ImuPanel.name} />
      </FloatPanel>
    );
  }
}
