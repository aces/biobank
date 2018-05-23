/**
 * Biobank Container Temperature Form
 *
 * @author Henri Rabalais
 * @version 1.0.0
 *
 * */

class TemperatureField extends React.Component {
  render() {
    return (
      <div className={this.props.className}>
        <div className='col-xs-6'> 
            <TextboxElement
              name='temperature'
              labelClass='col-xl-0'
              inputClass='col-lg-12'
              onUserInput={this.props.setContainerData}
              value={this.props.container.temperature}
            />  
        </div>
        <div className='col-xs-3'> 
          <ButtonElement
            label="Update"
            onUserInput={this.props.saveContainer}
            columnSize= 'col-lg-12'
          />
        </div>
        <div className='col-xs-3'> 
          <a onClick={()=>{this.props.toggle(); this.props.revertContainerData()}}>
            Cancel
          </a>
        </div>
      </div>
    );
  }
}

TemperatureField.propTypes = {
  setContainerData: React.PropTypes.func.isRequired,
  revertContainerData: React.PropTypes.func.isRequired,
  container: React.PropTypes.object.isRequired,
  saveContainer: React.PropTypes.func.isRequired,
  className: React.PropTypes.string
};

export default TemperatureField;
