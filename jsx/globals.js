import FormModal from 'FormModal';
import QuantityField from './quantityField';
import TemperatureField from './temperatureField';
import StatusField from './statusField';
import LocationField from './locationField';
import ContainerParentForm from './containerParentForm';

/**
 * Biobank Globals Component
 *
 * @author Henri Rabalais
 * @version 1.0.0
 *
 * */

class Globals extends React.Component {

  render() {
    let specimenTypeField;
    if (this.props.data.specimen) {
      specimenTypeField = (
        <div className="item">                                                
          <div className='field'>                                             
            Specimen Type
            <div className='value'>
              {this.props.options.specimenTypes[
                this.props.data.specimen.typeId
              ].type}
            </div>
          </div>
        </div>
      );
    }

    let containerTypeField = (
       <div className="item">                                                
         <div className='field'>                                             
           Container Type                                                              
           <div className='value'>                                           
             {this.props.options.containerTypes[
               this.props.data.container.typeId
             ].label}
           </div>                                                            
         </div>                                                              
       </div>                                                                
    );                                                                            

    let quantityField;                                                          
    if (this.props.data.specimen) {
      if (!this.props.edit.quantity) {                                             
        quantityField = (                                                         
          <div className="item">                                                  
            <div className='field'>                                               
              Quantity                                                            
              <div className='value'>                                             
                {this.props.data.specimen.quantity}                               
                {' '+this.props.options.specimenUnits[this.props.data.specimen.unitId].unit}
              </div>
            </div>
            <div
              className='action'
              title='Update Quantity'
            >
              <div                                                                
                className='action-button update'                                  
                onClick={() => this.props.toggle('quantity')}                       
              >                                                                   
                <span className='glyphicon glyphicon-chevron-right'/>             
              </div>                                                              
            </div>                                                                
          </div>                                                                  
        );                                                                        
      } else {                                                                    
        let units = this.props.options.specimenUnits[this.props.data.specimen.typeId];
        units = this.props.mapFormOptions(units, 'unit');
        quantityField = (
          <div className="item">
            <div className='field'>
              Quantity
              <QuantityField
                className='centered-horizontal'
                specimen={this.props.specimen}
                units={units}
                toggle={() => this.props.toggle('quantity')}
                setSpecimenData={this.props.setSpecimenData}
                revertSpecimenData={this.props.revertSpecimenData}
                saveSpecimen={this.props.saveSpecimen}
              />
            </div>
          </div>
        )
      }
    }

    let temperatureField;                                                        
    if (!this.props.edit.temperature) {                                     
      temperatureField = (                                                       
        <div className="item">                                                  
          <div className='field'>                                               
            Temperature                                                         
            <div className='value'>                                             
              {this.props.data.container.temperature + '°C'}                    
            </div>                                                              
          </div>                                                                
          <div                                                                  
            className='action'                                                  
            title='Update Temperature'                                          
          >                                                                     
            <span                                                               
              className='action-button update'                                  
              onClick={() => this.props.toggle('temperature')}                    
            >                                                                   
              <span className='glyphicon glyphicon-chevron-right'/>             
            </span>                                                             
          </div>                                                                
        </div>                                                                  
      );                                                                         
    } else {                                                                   
      temperatureField = (                                                      
        <div className="item">                                                  
          <div className='field'>                                               
            Temperature                                                         
            <TemperatureField                                                   
              className='centered-horizontal'                                   
              container={this.props.container}                                  
              toggle={() => this.props.toggle('temperature')}                     
              setContainerData={this.props.setContainerData}
              revertContainerData={this.props.revertContainerData}
              saveContainer={this.props.saveContainer}
            />                                                                  
          </div>                                                                
        </div>                                                                  
      )                                                                         
    }                                                                           
                                                                                
   let statusField;                                                             
   if (!this.props.edit.status) {                                          
     statusField = (                                                            
        <div className="item">                                                  
          <div className='field'>                                               
            Status                                                              
            <div className='value'>                                             
              {this.props.options.containerStati[this.props.data.container.statusId].status}
            </div>                                                              
          </div>                                                                
          <div                                                                  
            className='action'                                                  
            title='Update Status'                                               
          >                                                                     
            <span                                                               
              className='action-button update'                                  
              onClick={() => this.props.toggle('status')}                         
            >                                                                   
              <span className='glyphicon glyphicon-chevron-right'/>             
            </span>                                                             
          </div>                                                                
        </div>                                                                  
     );                                                                         
     } else {                                                                   
      let stati = this.props.mapFormOptions(this.props.options.containerStati, 'status');
      statusField = (                                                           
        <div className="item">                                                  
          <div className='field'>                                               
            Status                                                              
            <StatusField                                                        
              className='centered-horizontal'                                   
              container={this.props.container}                                  
              stati={stati}
              toggle={() => this.props.toggle('status')}                          
              setContainerData={this.props.setContainerData}                          
              revertContainerData={this.props.revertContainerData}                    
              saveContainer={this.props.saveContainer}                                
            />                                                                  
          </div>                                                                
        </div>                                                                  
      )                                                                         
    }                                                      

    let locationField;                                                           
    if (!this.props.edit.location) {                                        
      locationField = (                                                          
        <div className="item">                                                  
          <div className='field'>                                               
            Location                                                            
            <div className='value'>                                             
              {this.props.options.sites[this.props.data.container.locationId]}  
            </div>                                                              
          </div>                                                                
          <div                                                                  
            className='action'                                                  
            title='Update Status'                                               
          >                                                                     
            <span                                                               
              className='action-button update'                                  
              onClick={() => this.props.toggle('location')}                       
            >                                                                   
              <span className='glyphicon glyphicon-chevron-right'/>             
            </span>                                                             
          </div>                                                                
        </div>                                                                  
      );                                                                         
    } else {                                                                   
      locationField = (                                                         
        <div className="item">                                                  
          <div className='field'>                                               
            Location                                                            
            <LocationField                                                      
              className='centered-horizontal'                                   
              container={this.props.container}
              sites={this.props.options.sites}
              toggle={() => this.props.toggle('location')}                        
              setContainerData={this.props.setContainerData}                          
              revertContainerData={this.props.revertContainerData}                    
              saveContainer={this.props.saveContainer}                                
            />                                                                  
          </div>                                                                
        </div>                                                                  
      );                                                                         
    }                                                                           

    let originField = (
      <div className="item">                                                
        <div className='field'>                                             
          Origin                                                            
          <div className='value'>                                           
            {this.props.options.sites[this.props.data.container.originId]}  
          </div>                                                            
        </div>                                                              
      </div>                                                                
    );

    let creationDate = (
      <div className="item">                                                
        <div className='field'>                                             
          Creation Date                                                     
          <div className='value'>                                           
            {this.props.data.container.dateTimeCreate}                      
          </div>                                                            
        </div>                                                              
      </div>                                                                
    );

    let parentSpecimenField;
    if (this.props.data.specimen) {
      if (this.props.specimen.parentSpecimenId) {
        let specimenURL = loris.BaseURL='/biobank/specimen/?barcode=';
        let parentSpecimenFieldValue = (
          <a href={specimenURL+this.props.data.parentSpecimen.barcode}>
            {this.props.data.parentSpecimen.barcode}
          </a>
        );

        parentSpecimenField = (
          <div className='item'>
            <div className='field'>
            Parent Specimen
              <div className='value'>
                {parentSpecimenFieldValue || 'None'}
              </div>
            </div>
          </div>
        );
      }
    }

    //checks if parent container exists and returns static element with href      
    let parentContainerBarcodeValue;                                               
    if (this.props.data.container.parentContainerId) {                            
      let containerURL = loris.BaseURL+"/biobank/container/?barcode=";            
      parentContainerBarcodeValue = (                                             
        <div>                                                                     
          <a                                                                      
          href={containerURL+this.props.options.containersNonPrimary[             
            this.props.data.container.parentContainerId                           
          ].barcode}>                                                             
            {this.props.options.containersNonPrimary[                             
              this.props.data.container.parentContainerId                         
            ].barcode}                                                            
          </a>                                                                    
        </div>                                                                    
      );                                                                          
    }                                                                             

    let parentContainerField = (
      <div className="item">
        <div className='field'>
          Parent Container
          <div className='value'>
            {parentContainerBarcodeValue || 'None'}
          </div>
          {(parentContainerBarcodeValue && this.props.data.container.coordinate) ? 
          'Coordinate '+this.props.data.container.coordinate : null}
        </div>                                                                    
        <div                                                                      
          className='action'                                                      
          title='Move Container'                                                  
        >                                                                         
          <FormModal                                                              
            title='Update Parent Container'                                       
            buttonClass='action-button update'                                    
            buttonContent={<span className='glyphicon glyphicon-chevron-right'/>} 
          >                                                                       
            <ContainerParentForm
              container={this.props.container}
              containersNonPrimary={this.props.options.containersNonPrimary}      
              containerDimensions={this.props.options.containerDimensions}        
              containerCoordinates={this.props.options.containerCoordinates}      
              containerTypes={this.props.options.containerTypes}                  
              containerStati={this.props.options.containerStati}                  
              setContainerData={this.props.setContainerData}                            
              revertContainerData={this.props.revertContainerData}
              saveContainer={this.props.saveContainer}
            />                                                                    
          </FormModal>                                                            
        </div>                                                                    
      </div>                                                                      
    );                                                                            

    let candidateSessionField;
    if (this.props.data.specimen) {
      candidateSessionField = (
        <div className="item">                                                
            <div className='field'>                                             
              PSCID                                                             
              <div className='value'>                                           
                <a href={loris.BaseURL+'/'+this.props.data.specimen.candidateId}>
                  {this.props.data.candidate.PSCID}                             
                </a>                                                            
              </div>                                                            
            </div>                                                              
            <div className='field'>                                             
              Visit Label                                                       
              <div className='value'>                                           
                <a href={
                  loris.BaseURL+'/instrument_list/?candID='+
                  this.props.data.specimen.candidateId+'&sessionID='+
                  this.props.data.specimen.sessionId
                }>
                  {this.props.data.session.Visit_label}                         
                </a>                                                            
              </div>                                                            
            </div>                                                              
          </div>
        );
    }

    let fieldList = (                                                              
      <div className='list'>                                                  
        {specimenTypeField}
        {containerTypeField}
        {quantityField}
        {temperatureField}
        {statusField}
        {locationField}
        {originField}
        {parentSpecimenField}
        {parentContainerField}
        {candidateSessionField}
      </div>                                                                  
    );                                                  

    return (
      <div className="globals">                                                 
        {fieldList}
      </div>
    );
  }
}

Globals.propTypes = {
};

export default Globals;
